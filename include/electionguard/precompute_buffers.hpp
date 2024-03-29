#ifndef __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__

#include "electionguard/group.hpp"

#include <array>
#include <cstdint>
#include <electionguard/constants.h>
#include <electionguard/export.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

using std::begin;
using std::copy;
using std::end;
using std::make_unique;
using std::mutex;
using std::unique_ptr;

namespace electionguard
{
    /// <summary>
    /// This object holds the Triple for the entries in the precomputed triple_queue
    /// The three items contained in this object are a random exponent (exp),
    /// g ^ exp mod p (g_to_exp) and K ^ exp mod p (pubkey_to_exp - where K is
    /// the public key).
    /// </summary>
    class EG_INTERNAL_API Triple
    {
        unique_ptr<ElementModQ> exp;
        unique_ptr<ElementModP> g_to_exp;
        unique_ptr<ElementModP> pubkey_to_exp;

      public:
        explicit Triple(const ElementModP &publicKey) { generateTriple(publicKey); }
        Triple() {}
        Triple(unique_ptr<ElementModQ> exp, unique_ptr<ElementModP> g_to_exp,
               unique_ptr<ElementModP> pubkey_to_exp);

        Triple(const Triple &triple);
        Triple(Triple &&);
        ~Triple();

        Triple &operator=(const Triple &triple);
        Triple &operator=(Triple &&);

        unique_ptr<ElementModQ> get_exp() { return exp->clone(); }

        unique_ptr<ElementModP> get_g_to_exp() { return g_to_exp->clone(); }

        unique_ptr<ElementModP> get_pubkey_to_exp() { return pubkey_to_exp->clone(); }

        unique_ptr<Triple> clone();

      protected:
        void generateTriple(const ElementModP &publicKey);
    };

    /// <summary>
    /// This object holds the Quadruple for the entries in the precomputed
    /// quadruple_queue. The four items contained in this object are the
    /// first random exponent (exp1), the second random exponent (exp2)
    /// g ^ exp1 mod p (g_to_exp1) and (g ^ exp2 mod p) * (K ^ exp mod p)
    /// (g_to_exp2 mult_by_pubkey_to_exp1 - where K is the public key).
    /// </summary>
    class EG_INTERNAL_API Quadruple
    {
        unique_ptr<ElementModQ> exp1;
        unique_ptr<ElementModQ> exp2;
        unique_ptr<ElementModP> g_to_exp1;
        unique_ptr<ElementModP> g_to_exp2_mult_by_pubkey_to_exp1;

      public:
        explicit Quadruple(const ElementModP &publicKey) { generateQuadruple(publicKey); }
        Quadruple(){};
        Quadruple(unique_ptr<ElementModQ> exp1, unique_ptr<ElementModQ> exp2,
                  unique_ptr<ElementModP> g_to_exp1,
                  unique_ptr<ElementModP> g_to_exp2_mult_by_pubkey_to_exp1);
        Quadruple(const Quadruple &quadruple);
        Quadruple(Quadruple &&);
        ~Quadruple();

        Quadruple &operator=(const Quadruple &quadruple);
        Quadruple &operator=(Quadruple &&);

        unique_ptr<ElementModQ> get_exp1() { return exp1->clone(); }

        unique_ptr<ElementModQ> get_exp2() { return exp2->clone(); }

        unique_ptr<ElementModP> get_g_to_exp1() { return g_to_exp1->clone(); }

        unique_ptr<ElementModP> get_g_to_exp2_mult_by_pubkey_to_exp1()
        {
            return g_to_exp2_mult_by_pubkey_to_exp1->clone();
        }

        unique_ptr<Quadruple> clone();

      protected:
        void generateQuadruple(const ElementModP &publicKey);
    };

    /// <summary>
    /// This object holds the two Triples and a Quadruple of precomputed
    /// values that are used to speed up encryption of a selection.
    /// Since the values are precomputed it removes all the exponentiations
    /// from the ElGamal encryption of the selection as well as the
    /// computation of the Chaum Pedersen proof.
    /// </summary>
    class EG_INTERNAL_API TwoTriplesAndAQuadruple
    {
        unique_ptr<Triple> triple1;
        unique_ptr<Triple> triple2;
        unique_ptr<Quadruple> quad;

      public:
        explicit TwoTriplesAndAQuadruple() {}
        TwoTriplesAndAQuadruple(unique_ptr<Triple> in_triple1, unique_ptr<Triple> in_triple2,
                                unique_ptr<Quadruple> in_quad);
        TwoTriplesAndAQuadruple(const TwoTriplesAndAQuadruple &other);
        TwoTriplesAndAQuadruple(TwoTriplesAndAQuadruple &&);
        ~TwoTriplesAndAQuadruple();

        TwoTriplesAndAQuadruple &operator=(const TwoTriplesAndAQuadruple &other);
        TwoTriplesAndAQuadruple &operator=(TwoTriplesAndAQuadruple &&);

        unique_ptr<Triple> get_triple1() { return triple1->clone(); }

        unique_ptr<Triple> get_triple2() { return triple2->clone(); }

        unique_ptr<Quadruple> get_quad() { return quad->clone(); }

        unique_ptr<TwoTriplesAndAQuadruple> clone();
    };

    /// <summary>
    /// A singleton context for a collection of precomputed triples and quadruples.
    /// </summary>
    class EG_INTERNAL_API PrecomputeBufferContext
    {
      public:
        PrecomputeBufferContext(const PrecomputeBufferContext &) = delete;
        PrecomputeBufferContext(PrecomputeBufferContext &&) = delete;
        PrecomputeBufferContext &operator=(const PrecomputeBufferContext &) = delete;
        PrecomputeBufferContext &operator=(PrecomputeBufferContext &&) = delete;

      private:
        PrecomputeBufferContext() {}
        ~PrecomputeBufferContext() {}

      public:
        static PrecomputeBufferContext &getInstance()
        {
            static PrecomputeBufferContext instance;
            return instance;
        }

        /// <summary>
        /// The init method initializes the precompute and allows the queue
        /// size to be set.
        /// 
        /// <param name="size_of_queue">by default the quad queue size is 5000, so
        ///                             10000 triples, if the caller wants the
        ///                             queue size to be different then this
        ///                             parameter is used</param>
        /// </summary>
        /// 
        static void init(uint32_t size_of_queue = 0);

        /// <summary>
        /// The populate method populates the precomputations queues with
        /// values used by encryptSelection. The function is stopped by calling
        /// stop_populate. Pre-computed values are currently computed by generating
        /// two triples and a quad. We do this because two triples and a quad
        /// are need for an encryptSelection.The triple queue is twice the size of
        /// the quad queue. We use two different queues in case we need to
        /// make the stop more granular at some point, in other words currently
        /// if we call stop it will finish the two triples and a quad before
        /// stopping. If we want it to stop more granularly (for example after
        /// each queue item then we can do that but it makes the code a bit
        /// more complicated). In anticipation of possibly needing this we
        /// use two queues.
        ///
        /// <param name="elgamalPublicKey">the elgamal public key for the election</param>
        /// <returns>void</returns>
        /// </summary>
        ///
        static void populate(const ElementModP &elgamalPublicKey);

        /// <summary>
        /// The stop_populate method stops the population of the
        /// precomputations queues started by the populate method.
        /// <returns>void</returns>
        /// </summary>
        ///
        static void stop_populate();

        /// <summary>
        /// Get the currently set maximum queue size for the number
        /// of quadruples to generate. The number of triples in
        /// the triple_queue will be twice this.
        /// <returns>uint32_t</returns>
        /// </summary>
        static uint32_t get_max_queue_size();

        /// <summary>
        /// Get the current number of quadruples in the quadruple_queue,
        /// the number of triples in the triple_queue will be twice this.
        /// <returns>uint32_t</returns>
        /// </summary>
        static uint32_t get_current_queue_size();

        /// <summary>
        /// Get the next two triples and a quadruple from the queues.
        /// This method is called by encryptSelection in order to get
        /// the precomputed values to encrypt the selection and make a
        /// proof for it.
        /// <returns>std::unique_ptr<TwoTriplesAndAQuadruple></returns>
        /// </summary>
        static std::unique_ptr<TwoTriplesAndAQuadruple> getTwoTriplesAndAQuadruple();

        /// <summary>
        /// Get the next triple from the triple queue.
        /// This method is called by hashedElgamalEncrypt in order to get
        /// the precomputed value to perform the hashed elgamal encryption.
        /// <returns>std::unique_ptr<Triple></returns>
        /// </summary>
        static std::unique_ptr<Triple> getTriple();

        /// <summary>
        /// Empty the precomputed values queues.
        /// <returns>void</returns>
        /// </summary>
        static void empty_queues();

      private:
        uint32_t max = DEFAULT_PRECOMPUTE_SIZE;
        static std::mutex queue_lock;
        bool populate_OK = false;
        std::queue<std::unique_ptr<Triple>> triple_queue;
        std::queue<std::unique_ptr<TwoTriplesAndAQuadruple>> twoTriplesAndAQuadruple_queue;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_PRECOMPUTE_BUFFERS_HPP_INCLUDED__ */
