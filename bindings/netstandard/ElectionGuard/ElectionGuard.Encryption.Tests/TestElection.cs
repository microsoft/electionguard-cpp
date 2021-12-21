﻿using System;
using System.Diagnostics;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestElection
    {
        [Test]
        public void Test_Can_Deserialize_Ciphertext_Election_Context()
        {
            // Arrange
            const string data = "{\"crypto_base_hash\":\"B8CF9A8915BDB19C681AFBCDD1797F2CF360F723843D977D0E1B280CA2B24245\",\"crypto_extended_base_hash\":\"731052175CAE2EF7CFE0E827C65BB585CD8A9CB243320EB34C09B5DF3F75F982\",\"commitment_hash\":\"02\",\"manifest_hash\":\"02\",\"elgamal_public_key\":\"F258E409B1A130E00A3793555E0EAB2F560AA12CC01A3CB6B357035C6E734256B4D67877C018CB57AF150DDBBD0AC22B9D74C0B15C1AC80953086FDDFAAB7FC503022B61BE8C6E4FECD02136F4AFC68B51390D0E7E90661763455B8B1D79DDCBA82C7FFBA5BC7C28A323B2624FE7A72E693E45B0B88ACF1EBC4F2743AC0FBB9D3414070A37A7CD52B1C1443B69982B86DDCFEA3137C383A4C10705B7B2A244C93CD0179BD1203E5DCE19A078452C0525AA577D6214D1A849AF5BCE02581F381A872436497DA0245F10245604069562F98BE4F424227331252D955E8E77B42B20D3AB85530AC824FA4D5A8EDC4A6C7E310019BED8B8062B9A95B63CC126EFCC74D7512095183E2A2A99386B40210A9B1E9B2433F1C00D04DCCDA4CAE17E27456C6BD7D55C45C5F98514400EDA58D93A82E0D456C209087ED41106190F969B1DA4841F8EA26907FF4D29CAF05A4C5159AED129F980A06351B18CC88FB0954E958984070C0C8999D043BC68E1BFB8151F061103C9D89D9FDF709F747F816275463568FA6E18BAAD0C21A6C204C3795383F91EEECE312A4FCFD71B541718FF21F7D408D3A2572C889A7D25957BD7D206041452B60F513B24604CD8336C351D6B7A70BD39BEDBAD0B910D329230F4A3228DF1398CBE7EE7EEF49CB22C94BE32ED6C3F51B4F2C43A40BC5C217106CD7A0550BA12BB3D84643F90976ECFEF79614F2C46\",\"number_of_guardians\":1,\"quorum\":1}";

            // Act
            var result = new CiphertextElectionContext(data);

            // Assert
            Assert.That(result.CommitmentHash.ToHex() == "02");
            Assert.That(result.ManifestHash.ToHex() == "02");
        }

        [Test]
        public void Test_Can_Assign_Extended_Data_To_Ciphertext_Election_Context()
        {
            // Arrange
            var extendedData = new LinkedList();
            extendedData.Append("ballot_base_uri", "http://something.vote/");

            // Act
            var context = new CiphertextElectionContext(
                3, 2,
                Constants.ONE_MOD_P, Constants.ONE_MOD_Q, Constants.ONE_MOD_Q,
                extendedData);

            var cachedValue = context.ExtendedData.GetValueAt(0);

            // Assert
            Assert.That(cachedValue == "http://something.vote/");
        }
    }
}
