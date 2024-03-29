#include "election.hpp"

#include "../../../src/electionguard/log.hpp"
#include "../../../src/electionguard/variant_cast.hpp"
#include "manifest.hpp"

#include <electionguard/election.hpp>
#include <electionguard/manifest.hpp>

extern "C" {
#include "election.h"
}

using namespace electionguard;
using namespace std;

EG_API eg_electionguard_status_t eg_test_election_mocks_get_fake_ciphertext_election(
  eg_election_manifest_t *in_manifest, eg_element_mod_p_t *in_public_key,
  eg_internal_manifest_t **out_manifest, eg_ciphertext_election_context_t **out_context)
{
    try {
        auto *manifest = AS_TYPE(Manifest, in_manifest);
        auto *publicKey = AS_TYPE(ElementModP, in_public_key);
        auto internal = make_unique<InternalManifest>(*manifest);
        auto context = electionguard::tools::generators::ElectionGenerator::getFakeContext(
          *internal, *publicKey);
        *out_manifest = AS_TYPE(eg_internal_manifest_t, internal.release());
        *out_context = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_mocks_get_fake_ciphertext_election", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}