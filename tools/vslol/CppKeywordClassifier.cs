//
// Lol Engine - VsLol add-in for Visual Studio
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Windows.Media;
using System.Text.RegularExpressions;
using System.Diagnostics; /* For debugging purposes */

using Microsoft.VisualStudio.Shell;
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
    internal IClassificationTypeRegistryService m_type_registry = null;
    [Import]
    internal IClassifierAggregatorService m_aggregator = null;
    [Import]
    internal IClassificationFormatMapService m_format_map = null;
#if FALSE
    [Import]
    internal ITextDocumentFactoryService m_textdoc_factory = null;
#endif
    [Import]
    internal SVsServiceProvider m_sp = null;

    internal static bool m_inprogress = false;

    public IClassifier GetClassifier(ITextBuffer buffer)
    {
        /* Avoid infinite recursion */
        if (m_inprogress)
            return null;

#if FALSE
        if (m_textdoc_factory != null)
        {
            ITextDocument doc;
            m_textdoc_factory.TryGetTextDocument(buffer, out doc);
            /* print doc.FilePath */
        }
#endif

        /* Try to guess whether this is a Lol Engine project */
        EnvDTE.DTE dte = m_sp.GetService(typeof(EnvDTE.DTE)) as EnvDTE.DTE;
        bool islolengine = false;
        if (dte.Solution.FullName.Contains("Lol.sln"))
            islolengine = true;

        LolGenericFormat.SetRegistry(m_type_registry, m_format_map);

        try
        {
            m_inprogress = true;
            return buffer.Properties.GetOrCreateSingletonProperty<CppKeywordClassifier>(delegate { return new CppKeywordClassifier(m_type_registry, m_aggregator.GetClassifier(buffer), buffer.ContentType, islolengine); });
        }
        finally { m_inprogress = false; }
    }
}

class CppKeywordClassifier : IClassifier
{
    private IClassifier m_classifier;
    private bool m_inprogress, m_islolengine;

    private IClassificationType m_types_type, m_constant_type, m_normal_type;
    private Regex m_types_regex, m_constant_regex, m_normal_regex;

    private static string[] m_all_types =
    {
        "void|bool|int|signed|unsigned|char|short|long|float|double",
        "class|struct|union|template|namespace|typename|typedef",
        "inline|restrict|export|explicit|mutable",
        "static|register|auto|volatile|extern|const"
    };

    private static string[] m_cpp_types =
    {
        "u?int(8|16|32|64|ptr|max)_t",
        "u?int_(least|fast)(8|16|32|64)_t",
        "(wchar|char16|char32|size|ssize|off|ptrdiff)_t",
        "(sig_atomic|fpos|clock|time|div|ldiv)_t",
        "va_list|jmp_buf|FILE|DIR",
        "__(int(8|16|32|64)|ptr(32|64)|m(64|128|128d|128i))",
    };

    private static string[] m_lol_types =
    {
        "ldouble|real|half",
        "(float|int)([234]|2x2|3x3|4x4)",
        "(f(16|128)||d|[ui](8|16||64)|r)(vec[234]|mat[234]|quat|cmplx)",
        "(|[dui])box[23]",
    };

    private static string[] m_csharp_types =
    {
        "var|string",
        "out|ref|internal|sealed|public|private|protected|override"
    };

    private static string[] m_lolfx_types =
    {
        "attribute|varying|uniform|in|out",
        "int|uint",
        "(|[dui])(vec|mat)[234]"
    };

    private static string[] m_all_constants =
    {
        "true|false"
    };

    private static string[] m_cpp_constants =
    {
        "NULL|nullptr",
        "EXIT_SUCCESS|EXIT_FAILURE",
        "M_(E|LOG(2|10)E|LN2|LN10|PI|PI_[24]|[12]_PI|2_SQRTPI|SQRT(2|1_2))",
        "SIG(HUP|INT|QUIT|ILL|TRAP|ABRT|FPE|KILL|USR1|SEGV|USR2|PIPE|ALRM)",
        "SIG(TERM|CHLD|CONT|STOP|TSTP|TTIN|TTOU)"
    };

    private static string[] m_lol_constants =
    {
        "(F|D|LD)_(PI|PI_[234]|[12]_PI|SQRT(2|3|1_2))",
    };

    private static string[] m_csharp_constants =
    {
        "null",
    };

    private static string[] m_lolfx_constants =
    {
        "gl_Position|gl_FragColor",
    };

    private static string[] m_all_normal =
    {
    };

    private static string[] m_cpp_normal =
    {
        "interface|delegate|event|finally",
        "gcnew|generic|initonly|property|sealed",
    };

    internal CppKeywordClassifier(IClassificationTypeRegistryService registry,
                                  IClassifier classifier,
                                  IContentType type,
                                  bool islolengine)
    {
        m_classifier = classifier;
        m_inprogress = false;
        m_islolengine = islolengine;

        m_types_type = registry.GetClassificationType("LolAnyType");
        m_normal_type = registry.GetClassificationType("LolAnyIdentifier");
        m_constant_type = registry.GetClassificationType("LolAnyConstant");

        List<string> types_list = m_all_types.ToList();
        List<string> constants_list = m_all_constants.ToList();
        List<string> normals_list = m_all_normal.ToList();

        if (type.IsOfType("c/c++"))
        {
            types_list = types_list.Concat(m_cpp_types).ToList();
            constants_list = constants_list.Concat(m_cpp_constants).ToList();
            normals_list = normals_list.Concat(m_cpp_normal).ToList();
        }

        if (type.IsOfType("csharp"))
        {
            types_list = types_list.Concat(m_csharp_types).ToList();
            constants_list = constants_list.Concat(m_csharp_constants).ToList();
        }

        if (type.IsOfType("lolfx"))
        {
            types_list = types_list.Concat(m_lolfx_types).ToList();
            constants_list = constants_list.Concat(m_lolfx_constants).ToList();
        }

        if (m_islolengine)
        {
            types_list = types_list.Concat(m_lol_types).ToList();
            constants_list = constants_list.Concat(m_lol_constants).ToList();
        }

        m_types_regex =
            new Regex("^(" + String.Join("|", types_list.ToArray()) + ")$");
        m_constant_regex =
            new Regex("^(" + String.Join("|", constants_list.ToArray()) + ")$");
        m_normal_regex =
            new Regex("^(" + String.Join("|", normals_list.ToArray()) + ")$");
    }

    public IList<ClassificationSpan> GetClassificationSpans(SnapshotSpan span)
    {
        List<ClassificationSpan> ret = new List<ClassificationSpan>();

        if (m_inprogress)
        {
            /* For some reason we can get called recursively when parsing a
             * string for the IntelliSense drop-down menu. There is information
             * on how to deal with it properly on the following SO page:
             * http://stackoverflow.com/q/3155598/111461
             * The crash can be reproduced by simply typing "vec2(" and waiting
             * for IntelliSense to spawn the menu. */
            ret.Add(new ClassificationSpan(span, m_constant_type));
            return ret;
        }

        m_inprogress = true;

        foreach (ClassificationSpan cs in m_classifier.GetClassificationSpans(span))
        {
            string cs_class = cs.ClassificationType.Classification.ToLower();

            /* Only apply our rules if we found a keyword or an identifier */
            if (cs_class == "keyword" || cs_class == "identifier")
            {
                if (m_types_regex.IsMatch(cs.Span.GetText()))
                {
                    ret.Add(new ClassificationSpan(cs.Span, m_types_type));
                    continue;
                }

                if (m_constant_regex.IsMatch(cs.Span.GetText()))
                {
                    ret.Add(new ClassificationSpan(cs.Span, m_constant_type));
                    continue;
                }

                if (m_normal_regex.IsMatch(cs.Span.GetText()))
                {
                    ret.Add(new ClassificationSpan(cs.Span, m_normal_type));
                    continue;
                }
            }

            ret.Add(cs);
        }

        m_inprogress = false;

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

    [Export(typeof(ClassificationTypeDefinition))]
    [Name(LolCppIdentifierFormat.m_name)]
    internal static ClassificationTypeDefinition LolCustomIdentifierType = null;
}

[Export(typeof(EditorFormatDefinition))]
[ClassificationType(ClassificationTypeNames = LolCppTypeFormat.m_name)]
[Name(LolCppTypeFormat.m_name)]
[UserVisible(true)]
[Order(After = Priority.Default)] /* Override the Visual Studio classifiers */
internal sealed class LolCppTypeFormat : LolGenericFormat
{
    public const string m_name = "LolAnyType";
    public LolCppTypeFormat()
    {
        this.DisplayName = "C/C++ Types and Qualifiers (VsLol)";
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
    public const string m_name = "LolAnyConstant";
    public LolCppConstantFormat()
    {
        this.DisplayName = "C/C++ Constants (VsLol)";
        this.ForegroundColor = Colors.Magenta;
        this.ForegroundOpacity = 1.0;
        this.IsBold = true;
        //CopyStyleColor("User Types");
    }
}

[Export(typeof(EditorFormatDefinition))]
[ClassificationType(ClassificationTypeNames = LolCppIdentifierFormat.m_name)]
[Name(LolCppIdentifierFormat.m_name)]
[UserVisible(true)]
[Order(After = Priority.Default)] /* Override the Visual Studio classifiers */
internal sealed class LolCppIdentifierFormat : LolGenericFormat
{
    public const string m_name = "LolAnyIdentifier";
    public LolCppIdentifierFormat()
    {
        this.DisplayName = "C/C++ Identifiers (VsLol)";
        this.ForegroundColor = Colors.Silver;
        this.ForegroundOpacity = 1.0;
        this.IsBold = false;
        CopyStyleColor(PredefinedClassificationTypeNames.Identifier);
    }
}

} /* namespace lol */
