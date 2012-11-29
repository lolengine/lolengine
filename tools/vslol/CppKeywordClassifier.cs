using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Windows.Media;
using System.Text.RegularExpressions;

using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Text.Formatting;
using Microsoft.VisualStudio.Language.StandardClassification;
using Microsoft.VisualStudio.Utilities;

namespace Lol.VisualStudio.Plugin
{
    [Export(typeof(IClassifierProvider))]
    [ContentType("c/c++")]
    [ContentType("csharp")]
    [ContentType("lolfx")]
    internal class LolClassifierProvider : IClassifierProvider
    {
        [Import]
        internal IClassificationTypeRegistryService m_type_registry = null; /* Set via MEF */

        [Import]
        internal IClassificationFormatMapService m_format_map = null;

        public IClassifier GetClassifier(ITextBuffer buffer)
        {
            LolGenericFormat.SetRegistry(m_type_registry, m_format_map);

            return buffer.Properties.GetOrCreateSingletonProperty<CppKeywordClassifier>(delegate { return new CppKeywordClassifier(m_type_registry, buffer.ContentType); });
        }
    }

    class CppKeywordClassifier : IClassifier
    {
        private IClassificationType m_customclass_type;
        private Regex m_regex;

        internal CppKeywordClassifier(IClassificationTypeRegistryService registry,
                                      IContentType type)
        {
            m_customclass_type = registry.GetClassificationType("LolCustomClass");

            string tmp = @"\b(";
            tmp += "void|bool|int|unsigned|char|short|long|float|double|ldouble|";
            tmp += "class|struct|union|template|const|static|extern|volatile|inline|namespace|";
            if (type.IsOfType("lolfx"))
                tmp += "attribute|varying|uniform|in|out|";
            if (type.IsOfType("csharp"))
                tmp += "var|string|internal|sealed|public|private|protected|";
            if (!type.IsOfType("csharp"))
                tmp += "(f(16|128)||d|[ui](8|16||64)|r)(vec[234]|mat[234]|quat|cmplx)|";
            if (type.IsOfType("c/c++"))
            {
                tmp += "u?int(8|16|32|64|ptr)_t|";
                tmp += "real|half|explicit|typename|typedef|";
            }
            tmp += @")\b";
            m_regex = new Regex(tmp);
        }

        public IList<ClassificationSpan> GetClassificationSpans(SnapshotSpan span)
        {
            List<ClassificationSpan> ret = new List<ClassificationSpan>();

            string tmp = span.GetText();
            var matches = m_regex.Matches(tmp);
            foreach (Match m in matches)
            {
                Span newspan = new Span(span.Start.Position + m.Index, m.Length);
                SnapshotSpan newsnapshot = new SnapshotSpan(span.Snapshot, newspan);
                ret.Add(new ClassificationSpan(newsnapshot, m_customclass_type));
            }

            return ret;
        }

        public event EventHandler<ClassificationChangedEventArgs> ClassificationChanged;
    }

    internal class LolGenericFormat : ClassificationFormatDefinition
    {
        static IClassificationTypeRegistryService m_type_registry;
        static IClassificationFormatMapService m_format_map;

        public static void SetRegistry(IClassificationTypeRegistryService type_registry,
                                       IClassificationFormatMapService format_map)
        {
            m_type_registry = type_registry;
            m_format_map = format_map;
        }

        protected void CopyStyleColor(string category)
        {
            if (m_type_registry == null || m_format_map == null)
                return;

            var map = m_format_map.GetClassificationFormatMap("Text Editor");
            if (map == null)
                return;

            //string[] foo = { "Comment", "Keyword", "C/C++ User Keywords", "Call Return", "HTML Comment" , "User Types", "User Types (Type parameters)", "User Types (Value types)"};

            var type = m_type_registry.GetClassificationType(category);
            if (type == null)
                return;

            var prop = map.GetExplicitTextProperties(type);
            if (prop == null)
                return;

            var c1 = prop.ForegroundBrush as SolidColorBrush;
            if (c1 != null && c1.Color != Colors.Transparent)
                this.ForegroundColor = c1.Color;
            var c2 = prop.BackgroundBrush as SolidColorBrush;
            if (c2 != null && c2.Color != Colors.Transparent)
                this.BackgroundColor = c1.Color;
        }
    }

    internal static class LolClassifierClassificationDefinition
    {
        [Export(typeof(ClassificationTypeDefinition))]
        [Name(LolCppTypeFormat.m_name)]
        internal static ClassificationTypeDefinition LolCustomClassType = null;
    }

    [Export(typeof(EditorFormatDefinition))]
    [ClassificationType(ClassificationTypeNames = LolCppTypeFormat.m_name)]
    [Name(LolCppTypeFormat.m_name)]
    [UserVisible(true)]
    [Order(After = Priority.Default)] /* Override the Visual Studio classifiers */
    internal sealed class LolCppTypeFormat : LolGenericFormat
    {
        public const string m_name = "LolCustomClass";
        public LolCppTypeFormat()
        {
            this.DisplayName = "C/C++ Types and Qualifiers";
            this.ForegroundColor = Colors.Lime;
            this.IsBold = true;
            //CopyStyleColor("User Types");
        }
    }
}
