#pragma once

#include "Platform.h"
#include "UpdateMessage.h"

#include "wincore.h"
#include "controls.h"
#include "stdcontrols.h"

#include <sigslot.h>

class UpdateController;

class UpdateDialogWin32 : public has_slots<>
{
	public:
		UpdateDialogWin32(UpdateController* controller);
		~UpdateDialogWin32();

		void init();
		void exec();

		// implements UpdateObserver
		void updateError(const std::string& errorMessage);
		void updateProgress(int percentage);
		void updateFinished();

		LRESULT WINAPI windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void installWindowProc(CWnd* window);

		CWinApp m_app;
		CWnd m_window;
		CStatic m_progressLabel;
		CProgressBar m_progressBar;
		CButton m_finishButton;
		bool m_hadError;
};

