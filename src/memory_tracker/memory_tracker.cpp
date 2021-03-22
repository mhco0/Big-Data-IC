#include "memory_tracker.h"
#undef new

namespace qsbd {
    namespace mem_track {

        mem_stamp::mem_stamp(char const * file_name, int line_num): file_name(file_name), line_num(line_num){
        }

        mem_stamp::~mem_stamp(){
        }

        block_header *block_header::our_first_node = nullptr;

        block_header::block_header(size_t requested_size){
            my_prev_node = nullptr;
            my_next_node = nullptr;
            my_requested_size = requested_size;
            my_file_name = "[unknown]";
            my_line_num = 0;
            my_type_name = "[unknown]";
        }

        block_header::~block_header(){
        }
            

        void block_header::stamp(char const * file_name, int line_num, char const * type_name){
            my_file_name = file_name;
            my_line_num = line_num;
            my_type_name = type_name;
        }

        void block_header::add_node(block_header * node){
            assert(node != nullptr);
            assert(node->my_prev_node == nullptr);
            assert(node->my_next_node == nullptr);

            // If we have at least one node in the list ...        
            if (our_first_node != nullptr){
                // ... make the new node the first node's predecessor.
                assert(our_first_node->my_prev_node == nullptr);
                our_first_node->my_prev_node = node;
            }

            // Make the first node the new node's succesor.
            node->my_next_node = our_first_node;

            // Make the new node the first node.
            our_first_node = node;
        }

        void block_header::remove_node(block_header * node){
            assert(node != nullptr);
            assert(our_first_node != nullptr);

            // If target node is the first node in the list...
            if (our_first_node == node) {
                // ... make the target node's successor the first node.
                assert(our_first_node->my_prev_node == nullptr);
                our_first_node = node->my_next_node;
            }
            
            // Link target node's predecessor, if any, to its successor.
            if (node->my_prev_node != nullptr){
                node->my_prev_node->my_next_node = node->my_next_node;
            }
            
            // Link target node's successor, if any, to its predecessor.
            if (node->my_next_node != nullptr){
                node->my_next_node->my_prev_node = node->my_prev_node;
            }

            // Clear target node's previous and next pointers.
            node->my_prev_node = nullptr;
            node->my_next_node = nullptr;
        }

        size_t block_header::count_blocks(){
            size_t count = 0;
            block_header * cur_node = our_first_node;

            while (cur_node != nullptr){
                count++;
                cur_node = cur_node->my_next_node;
            }

            return count;
        }

        void block_header::get_blocks(block_header ** block_header_pp){
            block_header * cur_node = our_first_node;

            while (cur_node != nullptr){
                *block_header_pp = cur_node;
                block_header_pp++;

                cur_node = cur_node->my_next_node;
            }
        }

        bool block_header::type_greater_than(block_header * header1, block_header * header2){
            return (strcmp(header1->my_type_name, header2->my_type_name) > 0);
        }

        static user_chunk * get_user_address(prolog_chunk * p_prolog){
            char *pch_prolog = reinterpret_cast<char *>(p_prolog);
            char *pch_user = pch_prolog + OFFSET_user_chunk;
            user_chunk *p_user = reinterpret_cast<user_chunk *>(pch_user);
            return p_user;
        }

        static prolog_chunk * get_prolog_address(user_chunk * p_user){
            char *pch_user = reinterpret_cast<char *>(p_user);
            char *pch_prolog = pch_user - OFFSET_user_chunk;
            prolog_chunk *p_prolog = reinterpret_cast<prolog_chunk *>(pch_prolog);
            return p_prolog;
        }

        static block_header * get_header_address(prolog_chunk * p_prolog){
            char *pch_prolog = reinterpret_cast<char *>(p_prolog);
            char *pchHeader = pch_prolog + OFFSET_block_header;
            block_header *p_header = reinterpret_cast<block_header *>(pchHeader);
            return p_header;
        }

        static signature * get_signature_address(prolog_chunk * p_prolog){
            char * pch_prolog = reinterpret_cast<char *>(p_prolog);
            char * pch_signature = pch_prolog + OFFSET_signature;
            signature * p_signature = reinterpret_cast<signature *>(pch_signature);
            return p_signature;
        }

        void * track_malloc(size_t size) {
            // Allocate the memory, including space for the prolog.
            prolog_chunk * p_prolog = (prolog_chunk *) malloc(SIZE_prolog_chunk + size);
            
            // If the allocation failed, then return nullptr.
            if (p_prolog == nullptr) return nullptr;

            // Use placement new to construct the block header in place.
            block_header * p_block_header = new(p_prolog) block_header(size);
            
            // Link the block header into the list of extant block headers.
            block_header::add_node(p_block_header);
            
            // Use placement new to construct the signature in place.
            signature * p_signature = new(get_signature_address(p_prolog)) signature;
            
            // Get the offset to the user chunk and return it.
            user_chunk * p_user = get_user_address(p_prolog);
            
            return p_user;
        }

        /* ---------------------------------------- track_free */
        
        void track_free(void * address){
            // It's perfectly valid for "address" to be null; return if it is.
            if (address == nullptr) return;
        
            // Get the prolog address for this memory block.
            user_chunk * p_user = reinterpret_cast<user_chunk *>(address);    
            prolog_chunk * p_prolog = get_prolog_address(p_user);
        
            // Check the signature, and if it's invalid, return immediately.
            signature *p_signature = get_signature_address(p_prolog);
            if (!signature::is_valid_signature(p_signature)) return;
            
            // Destroy the signature.
            p_signature->~signature();
            p_signature = nullptr;

            // Unlink the block header from the list and destroy it.
            block_header *p_block_header = get_header_address(p_prolog);
            block_header::remove_node(p_block_header);
            p_block_header->~block_header();
            p_block_header = nullptr;

            // Free the memory block.    
            free(p_prolog);
        }

        void track_stamp(void * address, const mem_stamp& stamp, char const * type_name){
            // Get the header and signature address for this pointer.
            user_chunk * p_user = reinterpret_cast<user_chunk *>(address);
            prolog_chunk * p_prolog = get_prolog_address(p_user);
            block_header * p_header = get_header_address(p_prolog);
            signature * p_signature = get_signature_address(p_prolog);

            // If the signature is not valid, then return immediately.
            if (!signature::is_valid_signature(p_signature)) return;

            // "Stamp" the information onto the header.
            p_header->stamp(stamp.file_name, stamp.line_num, type_name);
        }

        void track_dump_blocks(){
            // Get an array of pointers to all extant blocks.
            size_t num_blocks = block_header::count_blocks();
            block_header ** pp_block_header = (block_header **) calloc(num_blocks, sizeof(*pp_block_header));
            block_header::get_blocks(pp_block_header);

            // Dump information about the memory blocks.
            printf("\n");
            printf("=====================\n");
            printf("Current Memory Blocks\n");
            printf("=====================\n");
            printf("\n");

            for (size_t i = 0; i < num_blocks; i++){
                block_header * p_block_header = pp_block_header[i];
                char const * type_name = p_block_header->get_type_name();
                size_t size = p_block_header->get_requested_size();
                char const * file_name = p_block_header->get_file_name();
                int line_num = p_block_header->get_line_num();
                printf("*** #%-6ld %5ld bytes %-50s\n", i, size, type_name);
                printf("... %s:%d\n", file_name, line_num);
            }

            // Clean up.
            free(pp_block_header);
        }

        static void summarize_memory_usage_for_type(mem_digest * pmem_digest, block_header ** pp_block_header, size_t start_post, size_t end_post){
            pmem_digest->type_name = pp_block_header[start_post]->get_type_name();
            pmem_digest->block_count = 0;
            pmem_digest->total_size = 0;

            for (size_t i = start_post; i < end_post; i++){
                pmem_digest->block_count++;
                pmem_digest->total_size += pp_block_header[i]->get_requested_size();
                assert(strcmp(pp_block_header[i]->get_type_name(), pmem_digest->type_name) == 0);
            }
        }

        void track_list_memory_usage(){
            // If there are no allocated blocks, then return now.
            size_t num_blocks = block_header::count_blocks();
            if (num_blocks == 0) return;

            // Get an array of pointers to all extant blocks.
            block_header ** pp_block_header = (block_header **) calloc(num_blocks, sizeof(*pp_block_header));
            block_header::get_blocks(pp_block_header);

            // Sort the blocks by type name.
            std::sort(pp_block_header, pp_block_header + num_blocks, block_header::type_greater_than);

            // Find out how many unique types we have.
            size_t num_unique_types = 1;
            for (size_t i = 1; i < num_blocks; i++){
                char const * prev_type_name = pp_block_header[i - 1]->get_type_name();
                char const * cur_type_name = pp_block_header[i]->get_type_name();
                if (strcmp(prev_type_name, cur_type_name) != 0) num_unique_types++;
            }

            // Create an array of "digests" summarizing memory usage by type.
            size_t start_post = 0;
            size_t unique_type_index = 0;
            mem_digest *p_mem_digest_array = (mem_digest *) calloc(num_unique_types, sizeof(*p_mem_digest_array));

            // yes, less than or *equal* to
            for (size_t i = 1; i <= num_blocks; i++) {  
                char const * prev_type_name = pp_block_header[i - 1]->get_type_name();
                char const * cur_type_name = (i < num_blocks) ? pp_block_header[i]->get_type_name() : "";
                
                if (strcmp(prev_type_name, cur_type_name) != 0){
                    size_t end_post = i;
                    summarize_memory_usage_for_type(p_mem_digest_array + unique_type_index, pp_block_header, start_post, end_post);
                    start_post = end_post;
                    unique_type_index++;
                }
            }
            assert(unique_type_index = num_unique_types);

            // Sort the digests by total memory usage.
            std::sort(p_mem_digest_array, p_mem_digest_array + num_unique_types, mem_digest::total_size_greater_than);

            // Compute the grand total memory usage.
            size_t grand_total_num_blocks = 0;
            size_t grand_total_size = 0;

            for (size_t i = 0; i < num_unique_types; i++){
                grand_total_num_blocks += p_mem_digest_array[i].block_count;
                grand_total_size += p_mem_digest_array[i].total_size;
            }

            // Dump the memory usage statistics.
            printf("\n");
            printf("-----------------------\n");
            printf("Memory Usage Statistics\n");
            printf("-----------------------\n");
            printf("\n");
            printf("%-50s%5s  %5s %7s %s \n", "allocated type", "blocks", "", "bytes", "");
            printf("%-50s%5s  %5s %7s %s \n", "--------------", "------", "", "-----", "");

            for (size_t i = 0; i < num_unique_types; i++){
                mem_digest *pMD = p_mem_digest_array + i;
                size_t block_count = pMD->block_count;
                double block_countPct = 100.0 * block_count / grand_total_num_blocks;
                size_t total_size = pMD->total_size;
                double total_size_pct = 100.0 * total_size / grand_total_size;

                printf("%-50s %5ld %5.1f%% %7ld %5.1f%%\n", pMD->type_name, block_count, block_countPct, total_size, total_size_pct);
            }
            printf("%-50s %5s %5s  %7s %s \n", "--------", "-----", "", "-------", "");
            printf("%-50s %5ld %5s  %7ld %s \n", "[totals]", grand_total_num_blocks, "", grand_total_size, "");

            // Clean up.
            free(pp_block_header);
            free(p_mem_digest_array);
        }

    } 
}

void * operator new(size_t size){
    void * address = qsbd::mem_track::track_malloc(size);
    if (address == nullptr) throw std::bad_alloc();
    return address;
}


void operator delete(void * address){
    qsbd::mem_track::track_free(address);
}


void * operator new[](size_t size){
    void * address = qsbd::mem_track::track_malloc(size);
    if (address == nullptr) throw std::bad_alloc();
    return address;
}

void operator delete[](void * address){
   qsbd::mem_track::track_free(address);
}
