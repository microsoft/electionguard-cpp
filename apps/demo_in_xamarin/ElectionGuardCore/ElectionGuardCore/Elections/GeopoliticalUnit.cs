﻿namespace ElectionGuardCore.Elections
{
    public class GeopoliticalUnit
    {
        public string ObjectId { get; set; }
        public string Name { get; set; }
        public ReportingUnitType? Type { get; set; }
        public ContactInformation ContactInfo { get; set; }
    }
}
