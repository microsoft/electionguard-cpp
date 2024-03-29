#include "electionguard/group.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/group.h"
}
#include "serialize.hpp"

#include <cstring>

using electionguard::dynamicCopy;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::G;
using electionguard::Log;
using electionguard::ONE_MOD_P;
using electionguard::ONE_MOD_Q;
using electionguard::P;
using electionguard::pow_mod_p;
using electionguard::Q;
using electionguard::R;
using electionguard::rand_q;
using electionguard::TWO_MOD_P;
using electionguard::TWO_MOD_Q;
using electionguard::ZERO_MOD_P;
using electionguard::ZERO_MOD_Q;

using std::make_unique;
using std::string;

using ConstantsSerializer = electionguard::Serialize::Constants;

#pragma region ElementModP

eg_electionguard_status_t eg_element_mod_p_new(const uint64_t in_data[MAX_P_LEN],
                                               eg_element_mod_p_t **out_handle)
{
    try {
        uint64_t data[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_P_SIZE);

        auto element = make_unique<ElementModP>(data);
        *out_handle = AS_TYPE(eg_element_mod_p_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_p_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_p_new_unchecked(const uint64_t in_data[MAX_P_LEN],
                                                         eg_element_mod_p_t **out_handle)
{
    try {
        uint64_t data[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_P_SIZE);

        auto element = make_unique<ElementModP>(data, true);
        *out_handle = AS_TYPE(eg_element_mod_p_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_p_new_unchecked", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_p_free(eg_element_mod_p_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElementModP, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_get_data(eg_element_mod_p_t *handle, uint64_t **out_data,
                                                    uint64_t *out_size)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *element = AS_TYPE(ElementModP, handle);
    *out_data = element->get();
    *out_size = (uint64_t)MAX_P_LEN;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_to_hex(eg_element_mod_p_t *handle, char **out_hex)
{
    try {
        auto hex_rep = AS_TYPE(ElementModP, handle)->toHex();
        *out_hex = dynamicCopy(hex_rep);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_p_to_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region ElementModQ

eg_electionguard_status_t eg_element_mod_q_new(const uint64_t in_data[MAX_Q_LEN],
                                               eg_element_mod_q_t **out_handle)
{
    try {
        uint64_t data[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_Q_SIZE);

        auto element = make_unique<ElementModQ>(data);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_q_new_unchecked(const uint64_t in_data[MAX_Q_LEN],
                                                         eg_element_mod_q_t **out_handle)
{
    try {
        uint64_t data[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_Q_SIZE);

        auto element = make_unique<ElementModQ>(data, true);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_new_unchecked", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_q_free(eg_element_mod_q_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElementModQ, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_get_data(eg_element_mod_q_t *handle, uint64_t **out_data,
                                                    uint64_t *out_size)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *element = AS_TYPE(ElementModQ, handle);
    *out_data = element->get();
    *out_size = (uint64_t)MAX_Q_LEN;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_to_hex(eg_element_mod_q_t *handle, char **out_hex)
{
    try {
        auto hex_rep = AS_TYPE(ElementModQ, handle)->toHex();
        *out_hex = dynamicCopy(hex_rep);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_to_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API eg_electionguard_status_t eg_element_mod_q_from_hex(char *in_hex,
                                                           eg_element_mod_q_t **out_handle)
{
    try {
        auto element = ElementModQ::fromHex(string(in_hex), false);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_from_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API eg_electionguard_status_t
eg_element_mod_q_from_hex_unchecked(char *in_hex, eg_element_mod_q_t **out_handle)
{
    try {
        auto element = ElementModQ::fromHex(string(in_hex), true);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_from_hex_unchecked", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Group Constants

eg_electionguard_status_t eg_element_mod_p_constant_g(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&G()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_constant_p(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&P()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_constant_r(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&R()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_element_mod_p_constant_zero_mod_p(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&ZERO_MOD_P()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_constant_one_mod_p(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&ONE_MOD_P()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_constant_two_mod_p(eg_element_mod_p_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(&TWO_MOD_P()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_constant_q(eg_element_mod_q_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(&Q()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_element_mod_q_constant_zero_mod_q(eg_element_mod_q_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(&ZERO_MOD_Q()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_constant_one_mod_q(eg_element_mod_q_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(&ONE_MOD_Q()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_constant_two_mod_q(eg_element_mod_q_t **out_constant_ref)
{
    *out_constant_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(&TWO_MOD_Q()));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region Group Math Functions

eg_electionguard_status_t eg_element_mod_q_pow_mod_p(eg_element_mod_p_t *base,
                                                     eg_element_mod_q_t *exponent,
                                                     eg_element_mod_p_t **out_handle)
{
    try {
        auto *b = AS_TYPE(ElementModP, base);
        auto *e = AS_TYPE(ElementModQ, exponent);
        auto result = pow_mod_p(*b, *e);

        *out_handle = AS_TYPE(eg_element_mod_p_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_pow_mod_p", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_q_rand_q_new(eg_element_mod_q_t **out_handle)
{
    try {
        auto random = rand_q();
        *out_handle = AS_TYPE(eg_element_mod_q_t, random.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_rand_q_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Group Serialization Functions

eg_electionguard_status_t eg_constant_to_json(char **out_data, uint64_t *out_size)
{
    try {
        auto result = ConstantsSerializer::toJson();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = (uint64_t)size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_constant_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
