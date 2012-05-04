#pragma once

#include <sigslot.h>

class UpdateDialogGtk;
class UpdateController;

/** A wrapper around UpdateDialogGtk which allows the GTK UI to
  * be loaded dynamically at runtime if the GTK libraries are
  * available.
  */
class UpdateDialogGtkWrapper : public sigslot::has_slots<>
{
	public:
		UpdateDialogGtkWrapper();

		/** Attempt to load and initialize the GTK updater UI.
		  * If this function returns false, other calls in UpdateDialogGtkWrapper
		  * may not be used.
		  */
		bool init(int argc, char** argv);
		void exec();

		void updateError(const std::string& errorMessage);
		void updateProgress(int percentage);
		void updateFinished();

	private:
		UpdateDialogGtk* m_dialog;
};

