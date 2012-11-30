//
// Lol Engine - VsLol add-in for Visual Studio
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

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

namespace lol
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
    internal IClassifierAggregatorService m_aggregator = null;
    [Import]
    internal IClassificationFormatMapService m_format_map = null;

    internal static bool m_inprogress = false;

    public IClassifier GetClassifier(ITextBuffer buffer)
    {
        /* Avoid infinite recursion */
        if (m_inprogress)
            return null;

        LolGenericFormat.SetRegistry(m_type_registry, m_format_map);

        try
        {
            m_inprogress = true;
            return buffer.Properties.GetOrCreateSingletonProperty<CppKeywordClassifier>(delegate { return new CppKeywordClassifier(m_type_registry, m_aggregator.GetClassifier(buffer), buffer.ContentType); });
        }
        finally { m_inprogress = false; }
    }
}

class CppKeywordClassifier : IClassifier
{
    private IClassifier m_classifier;

    private IClassificationType m_customclass_type, m_constant_type;
    private Regex m_customclass_regex, m_constant_regex;

    internal CppKeywordClassifier(IClassificationTypeRegistryService registry,
                                  IClassifier classifier,
                                  IContentType type)
    {
        m_classifier = classifier;

        /* Regex for types and specifiers */
        m_customclass_type = registry.GetClassificationType("LolCustomClass");

        string tmp = @"\b(";
        tmp += "void|bool|int|unsigned|char|short|long|float|double|ldouble|";
        tmp += "class|struct|union|template|const|static|extern|volatile|inline|namespace|";
        if (type.IsOfType("lolfx"))
            tmp += "attribute|varying|uniform|in|out|";
        if (type.IsOfType("csharp"))
            tmp += "var|out|ref|string|internal|sealed|public|private|protected|override|";
        if (!type.IsOfType("csharp"))
            tmp += "(f(16|128)||d|[ui](8|16||64)|r)(vec[234]|mat[234]|quat|cmplx)|";
        if (type.IsOfType("c/c++"))
        {
            tmp += "u?int(8|16|32|64|ptr)_t|";
            tmp += "(wchar|size|ssize)_t|";
            tmp += "real|half|explicit|typename|typedef|auto|";
        }
        tmp = tmp.Remove(tmp.Length - 1);
        tmp += @")\b";
        m_customclass_regex = new Regex(tmp);

        /* Regex for constant words */
        m_constant_type = registry.GetClassificationType("LolCppConstant");

        if (type.IsOfType("csharp"))
            m_constant_regex = new Regex(@"\b(null|true|false)\b");
        else if (type.IsOfType("c/c++"))
            m_constant_regex = new Regex(@"\b(NULL|nullptr|true|false|M_PI)\b");
        else if (type.IsOfType("lolfx"))
            m_constant_regex = new Regex(@"\b(gl_Position|gl_FragColor)\b");
        else
            m_constant_regex = new Regex(@"\b(NULL)\b");

    }

    public IList<ClassificationSpan> GetClassificationSpans(SnapshotSpan span)
    {
        List<ClassificationSpan> ret = new List<ClassificationSpan>();

        foreach (ClassificationSpan cs in m_classifier.GetClassificationSpans(span))
        {
            string cs_class = cs.ClassificationType.Classification.ToLower();

            /* Only apply our rules if we found a keyword or an identifier */
            if (cs_class == "keyword" || cs_class == "identifier")
            {
                if (m_customclass_regex.IsMatch(cs.Span.GetText()))
                {
                    ret.Add(new ClassificationSpan(cs.Span, m_customclass_type));
                    continue;
                }

                if (m_constant_regex.IsMatch(cs.Span.GetText()))
                {
                    ret.Add(new ClassificationSpan(cs.Span, m_constant_type));
                    continue;
                }
            }

            ret.Add(cs);
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
        {
            this.ForegroundColor = c1.Color;
            this.ForegroundOpacity = 1.0;
        }
        var c2 = prop.BackgroundBrush as SolidColorBrush;
        if (c2 != null && c2.Color != Colors.Transparent)
        {
            this.BackgroundColor = c2.Color;
            this.BackgroundOpacity = 1.0;
        }
    }
}

internal static class LolClassifierClassificationDefinition
{
    [Export(typeof(ClassificationTypeDefinition))]
    [Name(LolCppTypeFormat.m_name)]
    internal static ClassificationTypeDefinition LolCustomClassType = null;

    [Export(typeof(ClassificationTypeDefinition))]
    [Name(LolCppConstantFormat.m_name)]
    internal static ClassificationTypeDefinition LolCustomConstantType = null;
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
        this.ForegroundOpacity = 1.0;
        this.IsBold = true;
        //CopyStyleColor("User Types");
    }
}

[Export(typeof(EditorFormatDefinition))]
[ClassificationType(ClassificationTypeNames = LolCppConstantFormat.m_name)]
[Name(LolCppConstantFormat.m_name)]
[UserVisible(true)]
[Order(After = Priority.Default)] /* Override the Visual Studio classifiers */
internal sealed class LolCppConstantFormat : LolGenericFormat
{
    public const string m_name = "LolCppConstant";
    public LolCppConstantFormat()
    {
        this.DisplayName = "C/C++ Constants";
        this.ForegroundColor = Colors.Magenta;
        this.ForegroundOpacity = 1.0;
        this.IsBold = true;
        //CopyStyleColor("User Types");
    }
}

} /* namespace lol */
