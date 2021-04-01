#ifndef QSBD_MEMORY_TRACKER_H
#define QSBD_MEMORY_TRACKER_H
#include "../../thirdparty/nlohmann/json.hpp"
#include <typeinfo>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <new>

namespace qsbd{
    namespace mem_track{

        class mem_stamp{
        public:
            char const * const file_name;
            int const line_num;

            mem_stamp(char const * file_name, int line_num);

            ~mem_stamp();
            
            template<class T> 
            friend inline T* operator*(const mem_stamp& stamp, T * address);
        };

        template <class T> 
        inline T* operator*(const mem_stamp& stamp, T * address){
            track_stamp(address, stamp, typeid(decltype(address)).name());

            return address;
        }


        void * track_malloc(size_t size);
        void track_free(void * address);
        void track_stamp(void * address, const mem_stamp& stamp, char const * type_name);
        void track_dump_blocks(nlohmann::json& out);
        void track_list_memory_usage(nlohmann::json& out);

        class block_header{
        private:
            static block_header *our_first_node;

            block_header * my_prev_node;
            block_header * my_next_node;
            size_t my_requested_size;
            char const * my_file_name;
            int my_line_num;
            char const * my_type_name;

        public:  
            block_header(size_t requested_size);
            ~block_header();
        
            size_t get_requested_size() const { return my_requested_size; }
            char const * get_file_name() const { return my_file_name; }
            int get_line_num() const { return my_line_num; }
            char const * get_type_name() const { return my_type_name; }
        
            void stamp(char const * file_name, int line_num, char const * type_name);
        
            static void add_node(block_header * node);
            static void remove_node(block_header * node);
            static size_t count_blocks();
            static void get_blocks(block_header ** block_header_pp);
            static bool type_greater_than(block_header * header1, block_header * header2);
        };

        class signature{
        private: 
            static const unsigned int SIGNATURE1 = 0xCAFEBABE;
            static const unsigned int SIGNATURE2 = 0xFACEFACE;

            unsigned int my_signature1;
            unsigned int my_signature2;
                
        public:       
            signature() : my_signature1(SIGNATURE1), my_signature2(SIGNATURE2) {};
            ~signature() { my_signature1 = 0; my_signature2 = 0; }

            static bool is_valid_signature(const signature * p_prospective_signature){
                try{
                    if (p_prospective_signature->my_signature1 != SIGNATURE1) return false;
                    if (p_prospective_signature->my_signature2 != SIGNATURE2) return false;
                    return true;
                }

                catch (...){
                    return false;
                }
            }
        };

        /* -------------------- address conversion -------------------- */
        /* ------------------------------------------------------------ */

        /* We divide the memory blocks we allocate into two "chunks", the
        * "prolog chunk" where we store information about the allocation,
        * and the "user chunk" which we return to the caller to use.
        */

        const size_t ALIGNMENT = 4;

        /* If "value" (a memory size or offset) falls on an alignment boundary,
        * then just return it.  Otherwise return the smallest number larger
        * than "value" that falls on an alignment boundary.
        */    

        #define PAD_TO_ALIGNMENT_BOUNDARY(value) \
            ((value) + ((ALIGNMENT - ((value) % ALIGNMENT)) % ALIGNMENT))

        /* ---------------------------------------- chunk structs */
        
        /* We declare incomplete structures for each chunk, just to 
        * provide type safety.
        */

        
        struct prolog_chunk;
        struct user_chunk;

        const size_t SIZE_block_header = PAD_TO_ALIGNMENT_BOUNDARY(sizeof(block_header));
        const size_t SIZE_signature = PAD_TO_ALIGNMENT_BOUNDARY(sizeof(signature));

        const size_t OFFSET_block_header = 0;
        const size_t OFFSET_signature = OFFSET_block_header + SIZE_block_header;
        const size_t OFFSET_user_chunk = OFFSET_signature + SIZE_signature;
        
        const size_t SIZE_prolog_chunk = OFFSET_user_chunk;

        struct mem_digest{
            char const * type_name;
            int block_count;
            size_t total_size;

            static bool total_size_greater_than(const mem_digest& md1, const mem_digest& md2) { 
                return md1.total_size > md2.total_size;
            }
        };

    }
}

#define new qsbd::mem_track::mem_stamp(__FILE__, __LINE__) * new 
#endif