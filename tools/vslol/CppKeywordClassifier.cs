using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Windows.Media;
using System.Text.RegularExpressions;

using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Classification;
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
        internal IClassificationTypeRegistryService m_class_registry = null; /* Set via MEF */

        //[Import]
        //internal IClassificationFormatMapService m_lol = null;

        public IClassifier GetClassifier(ITextBuffer buffer)
        {
            //var test = m_lol.GetClassificationFormatMap("Text Editor");
            //string[] foo = { "Comment", "Keyword", "C/C++ User Keywords", "Call Return", "HTML Comment" };
            //foreach (var s in foo)
            //{
            //    var type = m_class_registry.GetClassificationType(s);
            //    if (type == null)
            //        continue;
            //    var prop = test.GetExplicitTextProperties(type);
            //    if (prop == null)
            //        continue;
            //    var c1 = prop.ForegroundBrush as SolidColorBrush;
            //    var c2 = prop.BackgroundBrush as SolidColorBrush;
            //    Logger.Info("[" + s + "]: " + c1.ToString() + " " + c2.ToString() + "\n");
            //}

            return buffer.Properties.GetOrCreateSingletonProperty<CppKeywordClassifier>(delegate { return new CppKeywordClassifier(m_class_registry, buffer.ContentType); });
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
            tmp += "void|int|unsigned|char|short|long|float|double|";
            tmp += "class|struct|template|const|static|volatile|inline|namespace|";
            if (type.IsOfType("lolfx"))
                tmp += "attribute|varying|uniform|in|out|";
            if (type.IsOfType("csharp"))
                tmp += "var|string|internal|sealed|public|private|";
            if (!type.IsOfType("csharp"))
                tmp += "vec2|vec3|vec4|quat|mat2|mat3|mat4|";
            if (type.IsOfType("c/c++"))
                tmp += "real|half|";
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
    internal sealed class LolCppTypeFormat : ClassificationFormatDefinition
    {
        public const string m_name = "LolCustomClass";
        public LolCppTypeFormat()
        {
            this.DisplayName = "C/C++ Types and Qualifiers";
            //this.BackgroundColor = Colors.BlueViolet;
            this.ForegroundColor = Colors.Lime;
            this.IsBold = true;
            //this.TextDecorations = System.Windows.TextDecorations.Underline;
        }
    }
}
