using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Package;
using Microsoft.VisualStudio.TextManager.Interop;
using Microsoft.VisualStudio.OLE.Interop;

namespace Lol.VisualStudio.Plugin
{
    class LolFxLanguageService : LanguageService
    {
        public override string GetFormatFilterList()
        {
            throw new NotImplementedException();
        }

        public override LanguagePreferences GetLanguagePreferences()
        {
            if (m_preferences == null)
            {
                m_preferences = new LanguagePreferences(this.Site,
                                 typeof(LolFxLanguageService).GUID, this.Name);
                m_preferences.Init();
            }
            return m_preferences;
        }

        public override IScanner GetScanner(IVsTextLines buffer)
        {
            if (m_scanner == null)
            {
                 m_scanner = new LolFxScanner(buffer);
            }
            return m_scanner;
        }

        public override string Name
        {
            get { return "LolFx"; }
        }

        public override AuthoringScope ParseSource(ParseRequest req)
        {
            return new LolFxAuthoringScope();
        }

        private LanguagePreferences m_preferences;
        private LolFxScanner m_scanner;

        internal class LolFxScanner : IScanner
        {
            public LolFxScanner(IVsTextBuffer buffer)
            {
                m_buffer = buffer;
            }

            bool IScanner.ScanTokenAndProvideInfoAboutIt(TokenInfo tokeninfo, ref int state)
            {
                while (m_offset < m_source.Length)
                {
                    if (m_source[m_offset] == ' ' || m_source[m_offset] == '\t')
                       {
                        ++m_offset;
                        continue;
                       }

                    tokeninfo.StartIndex = m_offset;
                    tokeninfo.EndIndex = m_offset;
                    tokeninfo.Type = TokenType.Unknown;
                    switch (state % 4)
                       {
                        case 0: tokeninfo.Color = TokenColor.Number; break;
                        case 1: tokeninfo.Color = TokenColor.Text; break;
                        case 2: tokeninfo.Color = TokenColor.Keyword; break;
                        case 3: tokeninfo.Color = TokenColor.Comment; break;
                    }
                    ++m_offset;
                    ++state;
                    return true;
                }

                return false;
            }

            enum State
            {
                Default,
                CComment,
                CppComment,
                String,
            }

            void IScanner.SetSource(string source, int offset)
            {
                m_source = source;
                m_offset = offset;
            }

            private IVsTextBuffer m_buffer;
            string m_source;
            int m_offset;
        }

        internal class LolFxAuthoringScope : AuthoringScope
        {
            public override string GetDataTipText(int line, int col, out TextSpan span)
            {
                span = new TextSpan();
                return null;
            }

            public override Declarations GetDeclarations(IVsTextView view,
                                                         int line,
                                                         int col,
                                                         TokenInfo info,
                                                         ParseReason reason)
            {
                return null;
            }

            public override string Goto(VSConstants.VSStd97CmdID cmd, IVsTextView textView, int line, int col, out TextSpan span)
            {
                span = new TextSpan();
                return null;
            }

            public override Methods GetMethods(int line, int col, string name)
            {
                return null;
            }
        }
    }
}

