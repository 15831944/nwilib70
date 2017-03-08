#include "stdafx.h"
#include "maskedit.h"


IMPLEMENT_DYNCREATE(CMaskEdit, CEdit)

CMaskEdit::CMaskEdit()
{
	m_bInit = FALSE;
	m_Prompt = "_";
	m_nCurrentChar = 0;
	m_bShiftDown = FALSE;
	SetCharCase(CASESENSITIVE);
	m_CharArray.RemoveAll();
}

CMaskEdit::~CMaskEdit()
{
	m_CharArray.RemoveAll();
}

BEGIN_MESSAGE_MAP(CMaskEdit, CEdit)
	// { AFX_MSG_MAP(CMaskEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	// }} AFX_MSG_MAP
END_MESSAGE_MAP()

void CMaskEdit::Setup(CWnd *Parent, unsigned int ID, CString strMask, BOOL bFindMode, 
					  const CString NULLString, int CharCase, BOOL bWithMask, LPCTSTR ErrMsg)
{	
	if(!this->SubclassDlgItem(ID, Parent))
	{
		CString Text;

		Text.Format("There is a problem when create %d Control", ID);
		MessageBox(Text);
		return;
	}

	SetFindMode(bFindMode);
	Init(strMask);
	SetCharCase(CharCase);
	SetWithMask(bWithMask);
	SetErrMsg(ErrMsg);
	SetNULLString(NULLString);
}

BOOL CMaskEdit::Init(CString strMask)
{
	m_bInit = FALSE;

	if(!Parse(strMask))
	{
		SetWindowText("Init Failed");
		EnableWindow(FALSE);
		return FALSE;
	}

	if(!SetMaskChars(strMask))
	{
		SetWindowText("SetInvalidChar Failed.");
		EnableWindow(FALSE);
		return FALSE;
	}

	m_nCurrentChar = FindNextChar(0, FALSE, TRUE);

	SetSel(m_nCurrentChar, m_nCurrentChar);

	EnableWindow(TRUE);
	m_bInit = TRUE;

	return TRUE;
}

void CMaskEdit::AdjustCharCase(UINT &nChar)
{
	switch(m_nCharCase)
	{
		case CASESENSITIVE:
		case NOCASE:
			break;
		case UPPERCASE:
			nChar = toupper(nChar);
			break;
		case LOWERCASE:
			nChar = tolower(nChar);
			break;
		default:
			break;
	}
}

void CMaskEdit::SetData(const CString Data)
{
	CString strText;
	CString strData;
	int iNextDataChar = 0;

	strText.Empty();

	for(int i = 1; i <= m_CharArray.GetSize(); i++)
	{
		if(m_CharArray.GetAt(i - 1)->IsStaticChar())
		{
			strText += m_CharArray.GetAt(i - 1)->GetValids();
			if(GetWithMask())
				iNextDataChar ++;
		}
		else
		{
			if(iNextDataChar < Data.GetLength())
			{
				strData = Data[iNextDataChar ++];
				if(!m_CharArray.GetAt(i - 1)->IsValid(strData))
					strData = m_Prompt;
			}
			else
				strData = m_Prompt;
			
			strText += strData[0];
		}
	}

	if(strText.IsEmpty())
		strText = GetNULLString();

	SetWindowText(strText);
}

CString CMaskEdit::GetData()
{
	int i;
	CString strText, strData, strNULL;

	GetWindowText(strText);

	for(i = 1; i <= m_CharArray.GetSize(); i++)
	{
		if(m_CharArray.GetAt(i - 1)->IsStaticChar())
		{
			if(GetWithMask())
				strData += m_CharArray.GetAt(i - 1)->GetValids();
		}
		else
		{
			if(strText.GetLength() >= i)
			{
				if(strText[i - 1] != m_Prompt)
					strData += strText[i - 1];
			}
			else
				strData += m_Prompt;
		}
	}

	// check to see whether data is null
	strText.Empty();  
	for(i = 0; i < m_CharArray.GetSize(); i++)
	{
		if(m_CharArray.GetAt(i)->IsStaticChar())
		{
			if(GetWithMask())
				strText += m_CharArray.GetAt(i)->GetValids();
		}
	}

	if(strData == GetNULLString() ||strData == strText) // To see if it is NULL.
		strData.Empty();

	return strData;
}

void CMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_TAB:
			break;
		case VK_BACK:
			if(m_nCurrentChar > 0)
			{
				int nNewText = FindNextChar(m_nCurrentChar - 1, FALSE, FALSE);
				if(nNewText >= 0)
				{
					SetSel(nNewText, nNewText + 1);
					ReplaceSel(m_Prompt);
					m_nCurrentChar = nNewText;
					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
			}
			break;
		default:
			int i, x, y;
			CString szText = "";

			GetSel(x, y);
			if(x != y)
			{
				for(i = x; i < y; i++)
				{
					if (m_CharArray.GetAt(i)->IsStaticChar())
						szText += m_CharArray.GetAt(i)->GetValids();
					else
						szText += m_Prompt;
				}
				
				ReplaceSel(szText);
				m_nCurrentChar = FindNextChar(x, FALSE, TRUE);
				SetSel(m_nCurrentChar, m_nCurrentChar);
			}

			if(m_nCurrentChar < m_CharArray.GetSize())
			{
				if(m_CharArray.GetAt(m_nCurrentChar)->IsValid((CString) (char) nChar))
				{
					int nNewText = FindNextChar(m_nCurrentChar + 1, FALSE, TRUE);
					
					SetSel(m_nCurrentChar, m_nCurrentChar + 1);
					AdjustCharCase(nChar);
					ReplaceSel((CString) (char) nChar);

					if(nNewText >= 0)
						m_nCurrentChar = nNewText;
					else
						m_nCurrentChar ++;

					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
			}
			break;
	}

	// CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CMaskEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_SHIFT)
		m_bShiftDown = FALSE;

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nNext = 0;
	CString szText = "";
	CWnd *pNextItem = NULL;

	switch(nChar)
	{
		case VK_TAB:
				pNextItem = (((CWnd*) GetParent())->GetNextDlgTabItem(this, m_bShiftDown));
				if(pNextItem->IsKindOf(RUNTIME_CLASS(CMaskEdit)))
					((CMaskEdit*) pNextItem)->m_bShiftDown = m_bShiftDown;
				((CWnd*) GetParent())->GetNextDlgTabItem(this, m_bShiftDown)->SetFocus();
				break;
		case VK_SHIFT:
				m_bShiftDown = TRUE;
				break;
		case VK_LEFT:
				nNext = FindNextChar(m_nCurrentChar - 1, FALSE, FALSE);
				if(nNext >= 0)
				{
					m_nCurrentChar = nNext;
					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
				break;
		case VK_RIGHT:
				nNext = FindNextChar(m_nCurrentChar + 1, FALSE, TRUE);
				if(nNext >= 0)
					m_nCurrentChar = nNext;
				else
					m_nCurrentChar = FindNextChar(m_nCurrentChar - 1, FALSE, FALSE) + 1;
				SetSel(m_nCurrentChar, m_nCurrentChar);
				break;
		case VK_HOME:
				if(m_bShiftDown)
					SetSel(0, m_nCurrentChar);
				else
				{
					m_nCurrentChar = FindNextChar(0, FALSE, TRUE);
					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
				break;
		case VK_END:
				if(m_bShiftDown)
				{
					int nLast = FindNextChar(m_CharArray.GetSize() - 1, FALSE, FALSE) + 1;
					SetSel(m_nCurrentChar, nLast);
				}
				else
				{
					m_nCurrentChar = FindNextChar(m_CharArray.GetSize() - 1, FALSE, FALSE) + 1;
					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
				break;
		case VK_INSERT:
				break;
		case VK_DELETE:
				int i, x, y;

				GetSel(x, y);
				if(x != y)
				{
					for(i = x; i < y; i++)
					{
						if(m_CharArray.GetAt(i)->IsStaticChar())
							szText += m_CharArray.GetAt(i)->GetValids();
						else
							szText += m_Prompt;
					}

					ReplaceSel(szText);
					m_nCurrentChar = FindNextChar(x, FALSE, TRUE);
					SetSel(m_nCurrentChar, m_nCurrentChar);
				}
				else
					if(m_nCurrentChar <= FindNextChar(m_CharArray.GetSize() - 1, FALSE, FALSE))
					{
						SetSel(m_nCurrentChar, m_nCurrentChar + 1);
						ReplaceSel(m_Prompt);
						nNext = FindNextChar(m_nCurrentChar + 1, FALSE, TRUE);
						
						if(nNext >= 0)
							m_nCurrentChar = nNext;
						else
							m_nCurrentChar ++;
						
						SetSel(m_nCurrentChar, m_nCurrentChar);
					}
				break;
		default:
				break;
	}

	// CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CMaskEdit::FindNextChar(int nStartPos, BOOL bStatic, BOOL bForward)
{
	int i;
	int nReturnVal = -1;

	if(bForward)
	{
		for(i = nStartPos; i < m_CharArray.GetSize(); i++)
		{
			if(m_CharArray.GetAt(i)->IsStaticChar() == bStatic)
			{
				nReturnVal = i;
				break;
			}
		}
	}
	else
	{
		for(i = nStartPos; i >= 0; i--)
		{
			if(m_CharArray.GetAt(i)->IsStaticChar() == bStatic)
			{
				nReturnVal = i;
				break;
			}
		}
	}
	
	return nReturnVal;
}

BOOL CMaskEdit::SetMaskChars(const CString strMask)
{
	CEditChar EditChar;
	CString strChars;
	CString strRangeLow;
	CString strSearch;
	BOOL bInside = FALSE;
	BOOL bInsideEscape = FALSE;
	BOOL bInsideRange = FALSE;
	int i;

	if(GetFindMode())
		strSearch = "%";

	m_CharArray.RemoveAll();

	for(i = 0; i < strMask.GetLength(); i++)
	{
		switch(strMask[i])
		{
			case '[': 
				bInside = TRUE;
				break;
			case ']':
				bInside = FALSE;
				strChars += strSearch;
				EditChar.Setup(strChars, FALSE);
				m_CharArray.Add(EditChar);
				strChars = "";
				break;
			case '-':
				if(bInside)
				{
					if(bInsideEscape)
					{
						strChars += "-";
						bInsideEscape = FALSE;
					}
					else
					{
						if(strChars.GetLength() == 0)
							return FALSE;
						bInsideRange = TRUE;
						strRangeLow = strChars.Right(1);
						strChars = strChars.Left(strChars.GetLength() - 1);
					}
				}
				else
				{
					EditChar.Setup((CString) (char) "-", TRUE); // treat "-" as a static char
					m_CharArray.Add(EditChar);
				}
				break;
			case '\\':
				if(bInside)
				{
					if(!bInsideEscape)
						bInsideEscape = TRUE;
					else
					{
						if(bInsideRange)
						{
							strChars += GetRange(strRangeLow, "\\");
							strRangeLow = "";
							bInsideEscape = FALSE;
						}
						else
						{
							strChars += "\\";
							bInsideEscape = FALSE;
						}
					}
				}
				else
				{
					EditChar.Setup((CString) (char) "\\", TRUE);
					m_CharArray.Add(EditChar);
				}
				break;
			default:
				if(bInside)
				{
					if(bInsideEscape)
						bInsideEscape = FALSE;
					
					if(bInsideRange)
					{
						strChars += GetRange(strRangeLow, (CString) (char) strMask[i]);
						strRangeLow = "";
						bInsideRange = FALSE;
					}
					else
						strChars += strMask[i];
				}
				else
				{
					EditChar.Setup((CString) (char) strMask[i], TRUE);
					m_CharArray.Add(EditChar);
				}
				break;
		}
	}
	
	SetLimitText(m_CharArray.GetSize());
	return TRUE;
}

CString CMaskEdit::GetRange(CString szLow, CString szHigh)
{
	CString szReturn = "";

	if(szLow.GetLength() == 1 && szHigh.GetLength() == 1)
	{
		char first = szLow[0];
		char last = szHigh[0];

		for(int i = first; i <= last; i++)
			szReturn += (CString) (char) i;
	}

	return szReturn;
}

BOOL CMaskEdit::Parse(const CString& strMask)
{
	BOOL bValid = TRUE;
	int nBracketState = 0;

	for(int i = 1; i <= strMask.GetLength(); i++)
	{
		switch(strMask[i - 1])
		{
			case '[':
				nBracketState++;
				break;
			case ']':
				nBracketState --;
				break;
			default:
				break;
		}

		if(nBracketState < 0 || nBracketState > 1)
			bValid = FALSE;
	}

	return (nBracketState == 0 && bValid) ? TRUE : FALSE;
}

BOOL CMaskEdit::SetPromptChar(CString PromptChar)
{
	if(PromptChar.GetLength() != 1)
		return FALSE;
	
	m_Prompt = PromptChar;
	return TRUE;
}

BOOL CMaskEdit::ValidData()
{
	return TRUE;
}

void CMaskEdit::OnSetFocus(CWnd *pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	SetSel(0, -1);
}

void CMaskEdit::OnKillFocus(CWnd *pNewWnd)
{
	if(ValidData())
		CEdit::OnKillFocus(pNewWnd);
	else
		CEdit::SetFocus();
}

void CMaskEdit::OnLButtonUp(UINT nFlags, CPoint Point)
{
	int x, y;

	GetSel(x, y);

	if(x == y)
	{
		m_nCurrentChar = x;
		SetSel(m_nCurrentChar, m_nCurrentChar);
	}

	CEdit::OnLButtonUp(nFlags, Point);
}

void CMaskEdit::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	SetSel(0, -1);
}

BOOL CMaskEdit::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
	{
		switch(pMsg->wParam)
		{
			case 'A':
				SetSel(0, -1);
				return TRUE;
			case 'C':
				Copy();
				return TRUE;
			case 'V':
				Paste();
				return TRUE;
			case 'X':
				Cut();
				return TRUE;
			case 'Z':
				Undo(); 
				return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}