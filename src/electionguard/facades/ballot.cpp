#include "electionguard/ballot.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>

extern "C" {
#include "electionguard/ballot.h"
}

using electionguard::BallotBoxState;
using electionguard::CiphertextBallot;
using electionguard::CiphertextBallotContest;
using electionguard::CiphertextBallotSelection;
using electionguard::dynamicCopy;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::ExtendedData;
using electionguard::Log;
using electionguard::PlaintextBallot;
using electionguard::PlaintextBallotContest;
using electionguard::PlaintextBallotSelection;
using electionguard::SubmittedBallot;

using std::make_unique;

#pragma region ExtendedData

EG_API eg_electionguard_status_t eg_extended_data_new(char *in_value, uint64_t in_length,
                                                      eg_extended_data_t **out_handle)
{
    try {
        auto extendedData = make_unique<ExtendedData>(string(in_value), in_length);
        *out_handle = AS_TYPE(eg_extended_data_t, extendedData.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_extended_data_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API eg_electionguard_status_t eg_extended_data_free(eg_extended_data_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ExtendedData, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_extended_data_get_value(eg_extended_data_t *handle,
                                                            char **out_value)
{
    try {
        auto value = AS_TYPE(ExtendedData, handle)->value;
        *out_value = dynamicCopy(value);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_extended_data_get_value", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API uint64_t eg_extended_data_get_length(eg_extended_data_t *handle)
{
    try {
        return AS_TYPE(ExtendedData, handle)->length;
    } catch (const exception &e) {
        Log::error("eg_extended_data_get_length", e);
        return 0;
    }
}

#pragma endregion

#pragma region PlaintextBallotSelection

eg_electionguard_status_t
eg_plaintext_ballot_selection_new(const char *in_object_id, uint64_t in_vote,
                                  bool in_is_placeholder_selection,
                                  eg_plaintext_ballot_selection_t **out_handle)
{
    try {
        auto selection =
          make_unique<PlaintextBallotSelection>(in_object_id, in_vote, in_is_placeholder_selection);
        *out_handle = AS_TYPE(eg_plaintext_ballot_selection_t, selection.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_selection_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_selection_new_with_extended_data(
  const char *in_object_id, const uint64_t in_vote, bool in_is_placeholder_selection,
  const char *in_extended_data_value, uint64_t in_extended_data_length,
  eg_plaintext_ballot_selection_t **out_handle)
{
    try {
        auto extendedData =
          make_unique<ExtendedData>(string(in_extended_data_value), in_extended_data_length);

        auto selection = make_unique<PlaintextBallotSelection>(
          in_object_id, in_vote, in_is_placeholder_selection, move(extendedData));
        *out_handle = AS_TYPE(eg_plaintext_ballot_selection_t, selection.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_selection_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(PlaintextBallotSelection, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *handle,
                                            char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(PlaintextBallotSelection, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_selection_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_plaintext_ballot_selection_get_vote(eg_plaintext_ballot_selection_t *handle)
{
    try {
        return AS_TYPE(PlaintextBallotSelection, handle)->getVote();
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_selection_get_vote", e);
        // note: returning zero here is implicitly a no-vote
        return 0;
    }
}

bool eg_plaintext_ballot_selection_get_is_placeholder(eg_plaintext_ballot_selection_t *handle)
{
    return AS_TYPE(PlaintextBallotSelection, handle)->getIsPlaceholder();
}

eg_electionguard_status_t
eg_plaintext_ballot_selection_get_extended_data(eg_plaintext_ballot_selection_t *handle,
                                                eg_extended_data_t **out_extended_data)
{
    try {
        auto *extendedData = AS_TYPE(PlaintextBallotSelection, handle)->getExtendedData();
        *out_extended_data = AS_TYPE(eg_extended_data_t, extendedData);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_selection_get_extended_data", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_plaintext_ballot_selection_is_valid(eg_plaintext_ballot_selection_t *handle,
                                            char *in_object_id)
{
    return AS_TYPE(PlaintextBallotSelection, handle)->isValid(string(in_object_id));
}

#pragma endregion

#pragma region CiphertextBallotSelection

eg_electionguard_status_t
eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextBallotSelection, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *handle,
                                             char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CiphertextBallotSelection, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_ciphertext_ballot_selection_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_ciphertext_ballot_selection_get_sequence_order(eg_ciphertext_ballot_selection_t *handle)
{
    try {
        return AS_TYPE(CiphertextBallotSelection, handle)->getSequenceOrder();
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_sequence_order", e);
        // note: returning zero here is an implicit failure
        return 0;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *handle,
                                                    eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *descriptionHash = AS_TYPE(CiphertextBallotSelection, handle)->getDescriptionHash();
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, descriptionHash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_ciphertext", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_ciphertext_ballot_selection_get_is_placeholder(eg_ciphertext_ballot_selection_t *handle)
{
    return AS_TYPE(CiphertextBallotSelection, handle)->getIsPlaceholder();
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_ciphertext(eg_ciphertext_ballot_selection_t *handle,
                                              eg_elgamal_ciphertext_t **out_ciphertext_ref)
{
    try {
        auto *ciphertext = AS_TYPE(CiphertextBallotSelection, handle)->getCiphertext();
        *out_ciphertext_ref = AS_TYPE(eg_elgamal_ciphertext_t, ciphertext);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_ciphertext", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_crypto_hash(eg_ciphertext_ballot_selection_t *handle,
                                               eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *hash = AS_TYPE(CiphertextBallotSelection, handle)->getCryptoHash();
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, hash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_nonce(eg_ciphertext_ballot_selection_t *handle,
                                         eg_element_mod_q_t **out_nonce_ref)
{
    try {
        auto *nonce = AS_TYPE(CiphertextBallotSelection, handle)->getNonce();
        *out_nonce_ref = AS_TYPE(eg_element_mod_q_t, nonce);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_nonce", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_proof(eg_ciphertext_ballot_selection_t *handle,
                                         eg_disjunctive_chaum_pedersen_proof_t **out_proof_ref)
{
    try {
        auto *proof = AS_TYPE(CiphertextBallotSelection, handle)->getProof();
        *out_proof_ref = AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t, proof);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_get_proof", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_crypto_hash_with(eg_ciphertext_ballot_selection_t *handle,
                                                eg_element_mod_q_t *in_encryption_seed,
                                                eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto *encryptionSeed = AS_TYPE(ElementModQ, in_encryption_seed);
        auto cryptoHash =
          AS_TYPE(CiphertextBallotSelection, handle)->crypto_hash_with(*encryptionSeed);
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_crypto_hash_with", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash)
{
    try {
        auto *encryptionSeed = AS_TYPE(ElementModQ, in_encryption_seed);
        auto *publicKey = AS_TYPE(ElementModP, in_public_key);
        auto *cryptoExtendedBaseHash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);
        return AS_TYPE(CiphertextBallotSelection, handle)
          ->isValidEncryption(*encryptionSeed, *publicKey, *cryptoExtendedBaseHash);
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_selection_is_valid_encryption", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region PlaintextBallotContest

eg_electionguard_status_t eg_plaintext_ballot_contest_free(eg_plaintext_ballot_contest_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(PlaintextBallotContest, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_plaintext_ballot_contest_get_object_id(eg_plaintext_ballot_contest_t *handle,
                                          char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(PlaintextBallotContest, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_contest_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_plaintext_ballot_contest_get_selections_size(eg_plaintext_ballot_contest_t *handle)
{
    try {
        auto contests = AS_TYPE(PlaintextBallotContest, handle)->getSelections();
        return contests.size();
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_contest_get_selections_size", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_contest_get_selection_at_index(
  eg_plaintext_ballot_contest_t *handle, size_t in_index,
  eg_plaintext_ballot_selection_t **out_selection_ref)
{
    try {
        auto selections = AS_TYPE(PlaintextBallotContest, handle)->getSelections();
        auto *selection = &selections.at(in_index).get();

        *out_selection_ref = AS_TYPE(eg_plaintext_ballot_selection_t, selection);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_contest_get_selections_size", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_plaintext_ballot_contest_is_valid(eg_plaintext_ballot_contest_t *handle,
                                          char *in_expected_object_id,
                                          uint64_t in_expected_num_selections,
                                          uint64_t in_expected_num_elected,
                                          uint64_t in_votes_allowed)

{
    try {
        return AS_TYPE(PlaintextBallotContest, handle)
          ->isValid(string(in_expected_object_id), in_expected_num_selections,
                    in_expected_num_elected, in_votes_allowed);
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_contest_is_valid", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region CiphertextBallotContest

eg_electionguard_status_t eg_ciphertext_ballot_contest_free(eg_ciphertext_ballot_contest_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextBallotContest, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_get_object_id(eg_ciphertext_ballot_contest_t *handle,
                                           char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CiphertextBallotContest, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_ciphertext_ballot_selection_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_ciphertext_ballot_contest_get_sequence_order(eg_ciphertext_ballot_contest_t *handle)
{
    try {
        return AS_TYPE(CiphertextBallotContest, handle)->getSequenceOrder();
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_sequence_order", e);
        // note: returning zero here is an implicit failure
        return 0;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_get_description_hash(eg_ciphertext_ballot_contest_t *handle,
                                                  eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *descriptionHash = AS_TYPE(CiphertextBallotContest, handle)->getDescriptionHash();
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, descriptionHash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_description_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_ciphertext_ballot_contest_get_selections_size(eg_ciphertext_ballot_contest_t *handle)
{
    auto contests = AS_TYPE(CiphertextBallotContest, handle)->getSelections();
    return contests.size();
}

eg_electionguard_status_t eg_ciphertext_ballot_contest_get_selection_at_index(
  eg_ciphertext_ballot_contest_t *handle, size_t in_index,
  eg_ciphertext_ballot_selection_t **out_selection_ref)
{
    try {
        auto selections = AS_TYPE(CiphertextBallotContest, handle)->getSelections();
        auto *selection = &selections.at(in_index).get();

        *out_selection_ref = AS_TYPE(eg_ciphertext_ballot_selection_t, selection);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_ciphertext_ballot_contest_get_selection_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_get_nonce(eg_ciphertext_ballot_contest_t *handle,
                                       eg_element_mod_q_t **out_nonce_ref)
{
    try {
        auto *nonce = AS_TYPE(CiphertextBallotContest, handle)->getNonce();
        *out_nonce_ref = AS_TYPE(eg_element_mod_q_t, nonce);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_nonce", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_contest_get_ciphertext_accumulation(
  eg_ciphertext_ballot_contest_t *handle, eg_elgamal_ciphertext_t **out_ciphertext_accumulation_ref)
{
    try {
        auto *ciphertext_accumulation =
          AS_TYPE(CiphertextBallotContest, handle)->getCiphertextAccumulation();
        *out_ciphertext_accumulation_ref =
          AS_TYPE(eg_elgamal_ciphertext_t, ciphertext_accumulation);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_ciphertext_accumulation", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_get_crypto_hash(eg_ciphertext_ballot_contest_t *handle,
                                             eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *hash = AS_TYPE(CiphertextBallotContest, handle)->getCryptoHash();
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, hash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_get_proof(eg_ciphertext_ballot_selection_t *handle,
                                       eg_disjunctive_chaum_pedersen_proof_t **out_proof_ref)
{
    try {
        auto *proof = AS_TYPE(CiphertextBallotContest, handle)->getProof();
        *out_proof_ref = AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t, proof);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_get_proof", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_crypto_hash_with(eg_ciphertext_ballot_contest_t *handle,
                                              eg_element_mod_q_t *in_encryption_seed,
                                              eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto *encryptionSeed = AS_TYPE(ElementModQ, in_encryption_seed);
        auto cryptoHash =
          AS_TYPE(CiphertextBallotContest, handle)->crypto_hash_with(*encryptionSeed);
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_crypto_hash_with", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_contest_aggregate_nonce(eg_ciphertext_ballot_contest_t *handle,
                                             eg_element_mod_q_t **out_aggregate_nonce)
{
    try {
        auto aggregateNonce = AS_TYPE(CiphertextBallotContest, handle)->aggregateNonce();
        *out_aggregate_nonce = AS_TYPE(eg_element_mod_q_t, aggregateNonce.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_crypto_hash_with", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_contest_elgamal_accumulate(
  eg_ciphertext_ballot_contest_t *handle, eg_elgamal_ciphertext_t **out_ciphertext_accumulation)
{
    try {
        auto accumulation = AS_TYPE(CiphertextBallotContest, handle)->elgamalAccumulate();
        *out_ciphertext_accumulation = AS_TYPE(eg_elgamal_ciphertext_t, accumulation.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_elgamal_accumulate", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_ciphertext_ballot_contest_is_valid_encryption(
  eg_ciphertext_ballot_contest_t *handle, eg_element_mod_q_t *in_encryption_seed,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash)
{
    try {
        auto *encryptionSeed = AS_TYPE(ElementModQ, in_encryption_seed);
        auto *publicKey = AS_TYPE(ElementModP, in_public_key);
        auto *cryptoExtendedBaseHash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);
        return AS_TYPE(CiphertextBallotContest, handle)
          ->isValidEncryption(*encryptionSeed, *publicKey, *cryptoExtendedBaseHash);
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_contest_is_valid_encryption", e);
        return false;
    }
}

#pragma endregion

#pragma region PlaintextBallot

eg_electionguard_status_t eg_plaintext_ballot_free(eg_plaintext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(PlaintextBallot, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_plaintext_ballot_get_object_id(eg_plaintext_ballot_t *handle,
                                                            char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(PlaintextBallot, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_get_style_id(eg_plaintext_ballot_t *handle,
                                                           char **out_style_id)
{
    try {
        auto ballotStyle = AS_TYPE(PlaintextBallot, handle)->getStyleId();
        *out_style_id = dynamicCopy(ballotStyle);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_get_style_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_plaintext_ballot_get_contests_size(eg_plaintext_ballot_t *handle)
{
    auto contests = AS_TYPE(PlaintextBallot, handle)->getContests();
    return contests.size();
}

eg_electionguard_status_t
eg_plaintext_ballot_get_contest_at_index(eg_plaintext_ballot_t *handle, size_t in_index,
                                         eg_plaintext_ballot_contest_t **out_contest_ref)
{
    try {
        auto contests = AS_TYPE(PlaintextBallot, handle)->getContests();
        auto *contest = &contests.at(in_index).get();

        *out_contest_ref = AS_TYPE(eg_plaintext_ballot_contest_t, contest);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_get_contest_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_from_json(char *in_data,
                                                        eg_plaintext_ballot_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = PlaintextBallot::fromJson(data);

        *out_handle = AS_TYPE(eg_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                        eg_plaintext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = PlaintextBallot::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                                           eg_plaintext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = PlaintextBallot::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_from_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *handle,
                                                      char **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(PlaintextBallot, handle);
        auto data_string = domain_type->toJson();
        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *handle,
                                                      uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(PlaintextBallot, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_to_msgpack(eg_plaintext_ballot_t *handle,
                                                         uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(PlaintextBallot, handle);
        auto data_bytes = domain_type->toMsgPack();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_plaintext_ballot_to_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region CiphertextBallot

eg_electionguard_status_t eg_ciphertext_ballot_free(eg_ciphertext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextBallot, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_ballot_get_object_id(eg_ciphertext_ballot_t *handle,
                                                             char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CiphertextBallot, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_get_style_id(eg_ciphertext_ballot_t *handle,
                                                            char **out_style_id)
{
    try {
        auto ballotStyle = AS_TYPE(CiphertextBallot, handle)->getStyleId();
        *out_style_id = dynamicCopy(ballotStyle);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_ciphertext_ballot_get_style_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_get_manifest_hash(eg_ciphertext_ballot_t *handle,
                                       eg_element_mod_q_t **out_manifest_hash_ref)
{
    try {
        auto *manifestHash = AS_TYPE(CiphertextBallot, handle)->getManifestHash();
        *out_manifest_hash_ref = AS_TYPE(eg_element_mod_q_t, manifestHash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_get_manifest_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_get_ballot_code_seed(eg_ciphertext_ballot_t *handle,
                                          eg_element_mod_q_t **out_ballot_code_seed_ref)
{
    try {
        auto *codeSeed = AS_TYPE(CiphertextBallot, handle)->getBallotCodeSeed();
        *out_ballot_code_seed_ref = AS_TYPE(eg_element_mod_q_t, codeSeed);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_ciphertext_ballot_get_ballot_code_seed", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_ciphertext_ballot_get_contests_size(eg_ciphertext_ballot_t *handle)
{
    auto contests = AS_TYPE(CiphertextBallot, handle)->getContests();
    return contests.size();
}

eg_electionguard_status_t
eg_ciphertext_ballot_get_contest_at_index(eg_ciphertext_ballot_t *handle, size_t in_index,
                                          eg_ciphertext_ballot_contest_t **out_contest_ref)
{
    try {
        auto contests = AS_TYPE(CiphertextBallot, handle)->getContests();
        auto *contest = &contests.at(in_index).get();

        *out_contest_ref = AS_TYPE(eg_ciphertext_ballot_contest_t, contest);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_ciphertext_ballot_get_contest_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_get_ballot_code(eg_ciphertext_ballot_t *handle,
                                     eg_element_mod_q_t **out_ballot_code_ref)
{
    try {
        auto *code = AS_TYPE(CiphertextBallot, handle)->getBallotCode();
        *out_ballot_code_ref = AS_TYPE(eg_element_mod_q_t, code);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_tracking_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_ciphertext_ballot_get_timestamp(eg_ciphertext_ballot_t *ciphertext)
{
    try {
        auto timestamp = AS_TYPE(CiphertextBallot, ciphertext)->getTimestamp();
        return timestamp;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_timestamp", e);
        return 0;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_get_nonce(eg_ciphertext_ballot_t *handle,
                                                         eg_element_mod_q_t **out_nonce_ref)
{
    try {
        auto *nonce = AS_TYPE(CiphertextBallot, handle)->getNonce();
        *out_nonce_ref = AS_TYPE(eg_element_mod_q_t, nonce);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_nonce", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_get_crypto_hash(eg_ciphertext_ballot_t *handle,
                                                               eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *cryptoHash = AS_TYPE(CiphertextBallot, handle)->getCryptoHash();
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, cryptoHash);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_crypto_hash_with(eg_ciphertext_ballot_t *handle,
                                      eg_element_mod_q_t *in_manifest_hash,
                                      eg_element_mod_q_t **out_hash_ref)
{
    try {
        auto *manifestHash = AS_TYPE(ElementModQ, in_manifest_hash);
        auto cryptoHash = AS_TYPE(CiphertextBallot, handle)->crypto_hash_with(*manifestHash);
        *out_hash_ref = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_crypto_hash_with", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_ciphertext_ballot_is_valid_encryption(eg_ciphertext_ballot_t *handle,
                                              eg_element_mod_q_t *in_encryption_seed,
                                              eg_element_mod_p_t *in_public_key,
                                              eg_element_mod_q_t *in_crypto_extended_base_hash)
{
    try {
        auto *ciphertext = AS_TYPE(CiphertextBallot, handle);
        auto *encryptionSeed = AS_TYPE(ElementModQ, in_encryption_seed);
        auto *public_key = AS_TYPE(ElementModP, in_public_key);
        auto *extended_hash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);

        return ciphertext->isValidEncryption(*encryptionSeed, *public_key, *extended_hash);
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_is_valid_encryption", e);
        return false;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_from_json(char *in_data,
                                                         eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = CiphertextBallot::fromJson(data);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                         eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CiphertextBallot::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                                            eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CiphertextBallot::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_from_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_json(eg_ciphertext_ballot_t *handle,
                                                       char **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_string = domain_type->toJson(false);

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_json_with_nonces(eg_ciphertext_ballot_t *handle,
                                                                   char **out_data,
                                                                   uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_string = domain_type->toJson(true);

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_json_with_nonces", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_bson(eg_ciphertext_ballot_t *handle,
                                                       uint8_t **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_bytes = domain_type->toBson(false);

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_bson_with_nonces(eg_ciphertext_ballot_t *handle,
                                                                   uint8_t **out_data,
                                                                   uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_bytes = domain_type->toBson(true);

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_bson_with_nonces", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_msgpack(eg_ciphertext_ballot_t *handle,
                                                          uint8_t **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_bytes = domain_type->toMsgPack(false);

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_to_msgpack_with_nonces(eg_ciphertext_ballot_t *handle, uint8_t **out_data,
                                            uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_bytes = domain_type->toMsgPack(true);

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_msgpack_with_nonces", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region SubmittedBallot

eg_electionguard_status_t eg_submitted_ballot_free(eg_submitted_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(SubmittedBallot, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_ballot_box_state_t eg_submitted_ballot_get_state(eg_submitted_ballot_t *handle)
{
    try {
        return (eg_ballot_box_state_t)AS_TYPE(SubmittedBallot, handle)->getState();
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_get_state", e);
        return ELECTIONGUARD_BALLOT_BOX_STATE_UNKNOWN;
    }
}

eg_electionguard_status_t eg_submitted_ballot_from(eg_ciphertext_ballot_t *in_ciphertext,
                                                   eg_ballot_box_state_t in_state,
                                                   eg_submitted_ballot_t **out_handle)
{
    try {
        auto *ciphertext = AS_TYPE(CiphertextBallot, in_ciphertext);
        auto submitted = SubmittedBallot::from(*ciphertext, (BallotBoxState)in_state);

        *out_handle = AS_TYPE(eg_submitted_ballot_t, submitted.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("submittedeg_submitted_ballot_from", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_from_json(char *in_data,
                                                        eg_submitted_ballot_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = SubmittedBallot::fromJson(data);

        *out_handle = AS_TYPE(eg_submitted_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                        eg_submitted_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = SubmittedBallot::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_submitted_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                                           eg_submitted_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = SubmittedBallot::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_submitted_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_from_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_to_json(eg_submitted_ballot_t *handle,
                                                      char **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(SubmittedBallot, handle);
        auto data_string = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_to_bson(eg_submitted_ballot_t *handle,
                                                      uint8_t **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(SubmittedBallot, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_submitted_ballot_to_msgpack(eg_submitted_ballot_t *handle,
                                                         uint8_t **out_data, uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(SubmittedBallot, handle);
        auto data_bytes = domain_type->toMsgPack();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_submitted_ballot_to_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
