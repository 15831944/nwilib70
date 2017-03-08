#ifndef __GROUP_H__
#define __GROUP_H__

class CGroupButton : public CWnd
{
protected:
	CStringArray m_StringArr;
	CWordArray	 m_IDArr;

public:
	CGroupButton(CWnd *Parent);
	CGroupButton();
	~CGroupButton() {}

	LPCTSTR GetCheckString();
	void AddButton(unsigned int ID, LPCTSTR String);
	void SetCheck(LPCTSTR String);
	void SetCheck(unsigned int ID, int nCheck);
	void ShowWindow(int nCmdShow);
	void EnableWindow(BOOL bEnable = TRUE);
};

#endif // __GROUP_H__