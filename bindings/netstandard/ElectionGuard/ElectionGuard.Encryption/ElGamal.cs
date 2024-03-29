﻿using System;
namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeElGamalKeyPair = NativeInterface.ElGamalKeyPair.ElGamalKeyPairHandle;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextHandle;

    /// <summary>
    /// An exponential ElGamal keypair
    /// </summary>
    public class ElGamalKeyPair : DisposableBase
    {
        /// <Summary>
        /// The ElGamal Public Key.
        /// </Summary>
        public unsafe ElementModP PublicKey
        {
            get
            {
                var status = NativeInterface.ElGamalKeyPair.GetPublicKey(
                    Handle, out NativeElementModP value);
                status.ThrowIfError();
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// The ElGamal Secret Key.
        /// </Summary>
        public unsafe ElementModQ SecretKey
        {
            get
            {
                var status = NativeInterface.ElGamalKeyPair.GetSecretKey(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeElGamalKeyPair Handle;

        unsafe internal ElGamalKeyPair(NativeElGamalKeyPair handle)
        {
            Handle = handle;
        }

        unsafe internal ElGamalKeyPair(ElementModQ secretKey)
        {
            var status = NativeInterface.ElGamalKeyPair.New(
                secretKey.Handle, out Handle);
            status.ThrowIfError();
        }

        /// <Summary>
        /// Make an elgamal keypair from a secret.
        /// </Summary>
        unsafe static public ElGamalKeyPair FromSecret(ElementModQ secretKey)
        {
            return new ElGamalKeyPair(secretKey);
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    /// <summary>
    /// An "exponential ElGamal ciphertext" (i.e., with the plaintext in the exponent to allow for
    /// homomorphic addition). Create one with `elgamal_encrypt`. Add them with `elgamal_add`.
    /// Decrypt using one of the supplied instance methods.
    /// </summary>
    public class ElGamalCiphertext : DisposableBase
    {
        /// <Summary>
        /// The pad value also referred to as A, a, 𝑎, or alpha in the spec.
        /// </Summary>
        public unsafe ElementModP Pad
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetPad(
                    Handle, out NativeElementModP value);
                status.ThrowIfError();
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// The data value also referred to as B, b, 𝛽, or beta in the spec.
        /// </Summary>
        public unsafe ElementModP Data
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetData(
                    Handle, out NativeElementModP value);
                status.ThrowIfError();
                return new ElementModP(value);
            }
        }

        /// <summary>
        /// Get the CryptoHash
        /// </summary>
        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetCryptoHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeElGamalCiphertext Handle;

        unsafe internal ElGamalCiphertext(NativeElGamalCiphertext handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// Decrypt the ciphertext directly using the provided secret key.
        ///
        /// This is a convenience accessor useful for some use cases.
        /// This method should not be used by consumers operating in live secret ballot elections.
        /// </Summary>
        unsafe public ulong? Decrypt(ElementModQ secretKey)
        {
            ulong plaintext = 0;
            var status = NativeInterface.ElGamalCiphertext.DecryptWithSecret(
                    Handle, secretKey.Handle, ref plaintext);
            status.ThrowIfError();
            return plaintext;
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    /// <summary>
    /// ElGamal Functions
    /// </summary>
    public static unsafe class Elgamal
    {
        /// <summary>
        /// Encrypts a message with a given random nonce and an ElGamal public key.
        ///
        /// <param name="plaintext"> Message to elgamal_encrypt; must be an integer in [0,Q). </param>
        /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
        /// <param name="publicKey"> ElGamal public key. </param>
        /// <returns>A ciphertext tuple.</returns>
        /// </summary>
        unsafe public static ElGamalCiphertext Encrypt(
            ulong plaintext, ElementModQ nonce, ElementModP publicKey)
        {
            var status = NativeInterface.ElGamal.Encrypt(
                    plaintext, nonce.Handle, publicKey.Handle,
                    out NativeElGamalCiphertext ciphertext);
            status.ThrowIfError();
            return new ElGamalCiphertext(ciphertext);
        }
    }
}
