/**
 * @file quantile_skecth.hpp
 * @brief QSBD quantile_skecth class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_QUANTILE_SKETCH
#define QSBD_QUANTILE_SKETCH

namespace qsbd {

    /** @class quantile_sketch\<Type\> 
     *  @brief A quantile_sketch interface
     * A interface for all that a quantile sketch should have
     * @tparam Type The type of the elements that the quantile sketch works with
     * @warning 
     * Although update and query aren't pure virtual member functions the should be implemented.
     * At least one of the update and one query method.
    */
    template<class Type>
    class quantile_sketch{
    private:
        // IDEA : change query to be relative to quantile after implements quantile_quadtree
        // IDEA : put cdf method to return Pr[X < value] = [rank(x) / # elements]
    public:
        /**
         * @brief A virtual destructor for all kinds of quantile_sketch
        */
        virtual ~quantile_sketch(){}

        /**
         * @brief A virtual init method this should be used unless the constructor was already called.
        */
        virtual void init(){}

        /**
         * @brief A virtual update method. Receives a @p elem and updates the sketch with it.
         * @param elem A element of the type supported by the sketch
        */
        virtual void update(Type elem){}

        /**
         * @brief A virtual update method. Receives a @p elem and a @p weight and updates the sketch with it.
         * @param elem A element of the type supported by the sketch
         * @param weight The weight associate with this @p elem
        */
        virtual void update(Type elem, int weight){}

        /**
         * @brief A virtual quantile method. Receives a @p quant and returns a element associate with this quantile.
         * @param quant The quantile to be queried
         * @return The element associete with the quantile @p quant
         * @warning 
         * The quantile should be in range [0..1)
        */
        virtual Type quantile(double quant){return Type();}

        /**
         * @brief A virtual query method. Receives a @p elem and returns the rank associate with this element.
         * @param elem The element to be query the rank
         * @return The rank(elem)
         * @note 
         * Unless specified, the rank should be defined as \f$rank(elem) = \sum _{y < elem} v_{y}\f$, where
         * \f$v_{y} = \f$ sum of all weights of y in the input. 
         */
        virtual int query(Type elem){return 0;}

        /**
         * @brief A pure virtual merge method. Recives a quantile sketch @p rhs performes a merge with this sketch, and returns a new quantile sketch
         * @param rhs A instance of a quantile_sketch\<Type\>
         * @return A pointer to a new instance of a quantile_sketch\<Type\>
         * @warning 
         * It's up to a user the function of delete this memory. 
         * @warning
         * The quantile_sketch passed as argument should be the same as this.
        */
        virtual quantile_sketch<Type>* merge(quantile_sketch<Type>& rhs) = 0;

        /**
         * @brief A pure virtual merge method. Recives a quantile sketch @p rhs performes a merge with this sketch, and save in this sketch
         * @param rhs A instance of a quantile_sketch\<Type\>
         * @warning
         * The quantile_sketch passed as argument should be the same as this.
        */
        virtual void inner_merge(quantile_sketch<Type>& rhs) = 0;
    };
}

#endif