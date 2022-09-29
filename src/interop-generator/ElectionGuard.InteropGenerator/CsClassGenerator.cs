﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 17.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace ElectionGuard.InteropGenerator
{
    using System.Linq;
    using System.Text;
    using System.Collections.Generic;
    using System.Text.Json;
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "17.0.0.0")]
    public partial class CsClassGenerator : CsClassGeneratorBase
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            
            #line 7 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"

static string ToSnakeCase(string text)
{
    if(text == null) {
        throw new ArgumentNullException(nameof(text));
    }
    if(text.Length < 2) {
        return text;
    }
    var sb = new StringBuilder();
    sb.Append(char.ToLowerInvariant(text[0]));
    for(int i = 1; i < text.Length; ++i) {
        char c = text[i];
        if(char.IsUpper(c)) {
            sb.Append('_');
            sb.Append(char.ToLowerInvariant(c));
        } else {
            sb.Append(c);
        }
    }
    return sb.ToString();
}

var className = EgClass.ClassName;

            
            #line default
            #line hidden
            this.Write("// DO NOT MODIFY THIS FILE\r\n// This file is generated via ElectionGuard.InteropGe" +
                    "nerator at /src/interop-generator\r\n\r\nusing System;\r\nusing System.Runtime.Interop" +
                    "Services;\r\n\r\nnamespace ElectionGuard\r\n{\r\n    public partial class ");
            
            #line 40 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(className));
            
            #line default
            #line hidden
            this.Write("\r\n    {\r\n        #region Properties\r\n");
            
            #line 43 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 foreach (var egProperty in EgClass.Properties) { 
        var name = egProperty.Name;
        var type = egProperty.Type;
        var description = egProperty.Description;
        var entryPoint = ToSnakeCase($"Eg{className}Get{name}");
        var isInternalType = egProperty.NativeHandleType != null;
        var isComplexType = type == "string" || isInternalType;
        
            
            #line default
            #line hidden
            this.Write("        [DllImport(\r\n            NativeInterface.DllName,\r\n            EntryPoint" +
                    " = \"");
            
            #line 53 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(entryPoint));
            
            #line default
            #line hidden
            this.Write("\",\r\n            CallingConvention = CallingConvention.Cdecl, \r\n            SetLas" +
                    "tError = true\r\n        )]\r\n        internal static extern ");
            
            #line 57 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(isComplexType ? "Status" : type));
            
            #line default
            #line hidden
            this.Write(" Get");
            
            #line 57 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(name));
            
            #line default
            #line hidden
            this.Write("(\r\n            NativeInterface.");
            
            #line 58 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(className));
            
            #line default
            #line hidden
            this.Write(".");
            
            #line 58 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(className));
            
            #line default
            #line hidden
            this.Write("Handle handle\r\n");
            
            #line 59 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 if (isInternalType) { 
            
            #line default
            #line hidden
            this.Write("            , out ");
            
            #line 60 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(egProperty.NativeHandleType));
            
            #line default
            #line hidden
            this.Write(" objectId\r\n");
            
            #line 61 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } else if (type == "string") { 
            
            #line default
            #line hidden
            this.Write("            , out IntPtr objectId\r\n");
            
            #line 63 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } 
            
            #line default
            #line hidden
            this.Write("        );\r\n\r\n        /// <Summary>\r\n        /// ");
            
            #line 67 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(description));
            
            #line default
            #line hidden
            this.Write("\r\n        /// </Summary>\r\n        public unsafe ");
            
            #line 69 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(type));
            
            #line default
            #line hidden
            this.Write(" ");
            
            #line 69 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(name));
            
            #line default
            #line hidden
            this.Write("\r\n        {\r\n            get\r\n            {\r\n");
            
            #line 73 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 if (isInternalType) { 
            
            #line default
            #line hidden
            this.Write("                var status = Get");
            
            #line 74 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(name));
            
            #line default
            #line hidden
            this.Write("(\r\n                    Handle, out ");
            
            #line 75 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(egProperty.NativeHandleType));
            
            #line default
            #line hidden
            this.Write(" value);\r\n                status.ThrowIfError();\r\n                return new ");
            
            #line 77 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(type));
            
            #line default
            #line hidden
            this.Write("(value);\r\n");
            
            #line 78 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } else if (type == "string") { 
            
            #line default
            #line hidden
            this.Write("                var status = Get");
            
            #line 79 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(name));
            
            #line default
            #line hidden
            this.Write("(Handle, out IntPtr value);\r\n                status.ThrowIfError();\r\n            " +
                    "    var data = Marshal.PtrToStringAnsi(value);\r\n                NativeInterface." +
                    "Memory.FreeIntPtr(value);\r\n                return data;\r\n");
            
            #line 84 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } else { 
            
            #line default
            #line hidden
            this.Write("                return Get");
            
            #line 85 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(name));
            
            #line default
            #line hidden
            this.Write("(Handle);\r\n");
            
            #line 86 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } 
            
            #line default
            #line hidden
            this.Write("            }\r\n        }\r\n\r\n");
            
            #line 90 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
} // foreach EgProperty
            
            #line default
            #line hidden
            this.Write("        #endregion\r\n\r\n        #region Methods\r\n");
            
            #line 94 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 foreach (var method in EgClass.Methods) { 
            
            #line default
            #line hidden
            this.Write("        /// <summary>\r\n        /// ");
            
            #line 96 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(method.Description));
            
            #line default
            #line hidden
            this.Write("\r\n        /// </summary>\r\n        public unsafe ");
            
            #line 98 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(method.ReturnType));
            
            #line default
            #line hidden
            this.Write(" ");
            
            #line 98 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(method.Name));
            
            #line default
            #line hidden
            this.Write("(\r\n            ");
            
            #line 99 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(string.Join(", ", method.Params.Select(p => $"{p.Type} {p.Name}"))));
            
            #line default
            #line hidden
            this.Write("\r\n        ) {\r\n            return NativeInterface.");
            
            #line 101 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(className));
            
            #line default
            #line hidden
            this.Write(".");
            
            #line 101 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(method.Name));
            
            #line default
            #line hidden
            this.Write("(\r\n                Handle, expectedObjectId);\r\n        }\r\n");
            
            #line 104 "C:\dev\ElectionGuard\electionguard-cpp\src\interop-generator\ElectionGuard.InteropGenerator\CsClassGenerator.tt"
 } // foreach method 
            
            #line default
            #line hidden
            this.Write("        #endregion\r\n    }\r\n}\r\n");
            return this.GenerationEnvironment.ToString();
        }
    }
    
    #line default
    #line hidden
    #region Base class
    /// <summary>
    /// Base class for this transformation
    /// </summary>
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "17.0.0.0")]
    public class CsClassGeneratorBase
    {
        #region Fields
        private global::System.Text.StringBuilder generationEnvironmentField;
        private global::System.CodeDom.Compiler.CompilerErrorCollection errorsField;
        private global::System.Collections.Generic.List<int> indentLengthsField;
        private string currentIndentField = "";
        private bool endsWithNewline;
        private global::System.Collections.Generic.IDictionary<string, object> sessionField;
        #endregion
        #region Properties
        /// <summary>
        /// The string builder that generation-time code is using to assemble generated output
        /// </summary>
        protected System.Text.StringBuilder GenerationEnvironment
        {
            get
            {
                if ((this.generationEnvironmentField == null))
                {
                    this.generationEnvironmentField = new global::System.Text.StringBuilder();
                }
                return this.generationEnvironmentField;
            }
            set
            {
                this.generationEnvironmentField = value;
            }
        }
        /// <summary>
        /// The error collection for the generation process
        /// </summary>
        public System.CodeDom.Compiler.CompilerErrorCollection Errors
        {
            get
            {
                if ((this.errorsField == null))
                {
                    this.errorsField = new global::System.CodeDom.Compiler.CompilerErrorCollection();
                }
                return this.errorsField;
            }
        }
        /// <summary>
        /// A list of the lengths of each indent that was added with PushIndent
        /// </summary>
        private System.Collections.Generic.List<int> indentLengths
        {
            get
            {
                if ((this.indentLengthsField == null))
                {
                    this.indentLengthsField = new global::System.Collections.Generic.List<int>();
                }
                return this.indentLengthsField;
            }
        }
        /// <summary>
        /// Gets the current indent we use when adding lines to the output
        /// </summary>
        public string CurrentIndent
        {
            get
            {
                return this.currentIndentField;
            }
        }
        /// <summary>
        /// Current transformation session
        /// </summary>
        public virtual global::System.Collections.Generic.IDictionary<string, object> Session
        {
            get
            {
                return this.sessionField;
            }
            set
            {
                this.sessionField = value;
            }
        }
        #endregion
        #region Transform-time helpers
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void Write(string textToAppend)
        {
            if (string.IsNullOrEmpty(textToAppend))
            {
                return;
            }
            // If we're starting off, or if the previous text ended with a newline,
            // we have to append the current indent first.
            if (((this.GenerationEnvironment.Length == 0) 
                        || this.endsWithNewline))
            {
                this.GenerationEnvironment.Append(this.currentIndentField);
                this.endsWithNewline = false;
            }
            // Check if the current text ends with a newline
            if (textToAppend.EndsWith(global::System.Environment.NewLine, global::System.StringComparison.CurrentCulture))
            {
                this.endsWithNewline = true;
            }
            // This is an optimization. If the current indent is "", then we don't have to do any
            // of the more complex stuff further down.
            if ((this.currentIndentField.Length == 0))
            {
                this.GenerationEnvironment.Append(textToAppend);
                return;
            }
            // Everywhere there is a newline in the text, add an indent after it
            textToAppend = textToAppend.Replace(global::System.Environment.NewLine, (global::System.Environment.NewLine + this.currentIndentField));
            // If the text ends with a newline, then we should strip off the indent added at the very end
            // because the appropriate indent will be added when the next time Write() is called
            if (this.endsWithNewline)
            {
                this.GenerationEnvironment.Append(textToAppend, 0, (textToAppend.Length - this.currentIndentField.Length));
            }
            else
            {
                this.GenerationEnvironment.Append(textToAppend);
            }
        }
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void WriteLine(string textToAppend)
        {
            this.Write(textToAppend);
            this.GenerationEnvironment.AppendLine();
            this.endsWithNewline = true;
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void Write(string format, params object[] args)
        {
            this.Write(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void WriteLine(string format, params object[] args)
        {
            this.WriteLine(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Raise an error
        /// </summary>
        public void Error(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Raise a warning
        /// </summary>
        public void Warning(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            error.IsWarning = true;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Increase the indent
        /// </summary>
        public void PushIndent(string indent)
        {
            if ((indent == null))
            {
                throw new global::System.ArgumentNullException("indent");
            }
            this.currentIndentField = (this.currentIndentField + indent);
            this.indentLengths.Add(indent.Length);
        }
        /// <summary>
        /// Remove the last indent that was added with PushIndent
        /// </summary>
        public string PopIndent()
        {
            string returnValue = "";
            if ((this.indentLengths.Count > 0))
            {
                int indentLength = this.indentLengths[(this.indentLengths.Count - 1)];
                this.indentLengths.RemoveAt((this.indentLengths.Count - 1));
                if ((indentLength > 0))
                {
                    returnValue = this.currentIndentField.Substring((this.currentIndentField.Length - indentLength));
                    this.currentIndentField = this.currentIndentField.Remove((this.currentIndentField.Length - indentLength));
                }
            }
            return returnValue;
        }
        /// <summary>
        /// Remove any indentation
        /// </summary>
        public void ClearIndent()
        {
            this.indentLengths.Clear();
            this.currentIndentField = "";
        }
        #endregion
        #region ToString Helpers
        /// <summary>
        /// Utility class to produce culture-oriented representation of an object as a string.
        /// </summary>
        public class ToStringInstanceHelper
        {
            private System.IFormatProvider formatProviderField  = global::System.Globalization.CultureInfo.InvariantCulture;
            /// <summary>
            /// Gets or sets format provider to be used by ToStringWithCulture method.
            /// </summary>
            public System.IFormatProvider FormatProvider
            {
                get
                {
                    return this.formatProviderField ;
                }
                set
                {
                    if ((value != null))
                    {
                        this.formatProviderField  = value;
                    }
                }
            }
            /// <summary>
            /// This is called from the compile/run appdomain to convert objects within an expression block to a string
            /// </summary>
            public string ToStringWithCulture(object objectToConvert)
            {
                if ((objectToConvert == null))
                {
                    throw new global::System.ArgumentNullException("objectToConvert");
                }
                System.Type t = objectToConvert.GetType();
                System.Reflection.MethodInfo method = t.GetMethod("ToString", new System.Type[] {
                            typeof(System.IFormatProvider)});
                if ((method == null))
                {
                    return objectToConvert.ToString();
                }
                else
                {
                    return ((string)(method.Invoke(objectToConvert, new object[] {
                                this.formatProviderField })));
                }
            }
        }
        private ToStringInstanceHelper toStringHelperField = new ToStringInstanceHelper();
        /// <summary>
        /// Helper to produce culture-oriented representation of an object as a string
        /// </summary>
        public ToStringInstanceHelper ToStringHelper
        {
            get
            {
                return this.toStringHelperField;
            }
        }
        #endregion
    }
    #endregion
}
