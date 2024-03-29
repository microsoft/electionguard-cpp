﻿using System;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestElGamal
    {
        [Test]
        public void Test_Elgamal_Encrypt_Simple()
        {
            // Arrange
            var nonce = Constants.ONE_MOD_Q;
            var secret = Constants.TWO_MOD_Q;
            var keyPair = ElGamalKeyPair.FromSecret(secret);
            var publicKey = keyPair.PublicKey;
            const ulong vote = 1UL;

            // Act
            var ciphertext = Elgamal.Encrypt(vote, nonce, publicKey);
            var plaintext = ciphertext.Decrypt(keyPair.SecretKey);

            // Assert
            Assert.That(plaintext == vote);
        }
    }
}
