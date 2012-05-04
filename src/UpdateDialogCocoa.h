#pragma once

#include <sigslot.h>

class UpdateDialogPrivate;
class UpdateController;

class UpdateDialogCocoa : public sigslot::has_slots<>
{
	public:
		UpdateDialogCocoa(UpdateController* controller);
		~UpdateDialogCocoa();

		void init();
		void exec();

		// slots for update notifications
		void updateError(const std::string& errorMessage);
		void updateProgress(int percentage);
		void updateFinished();

		static void* createAutoreleasePool();
		static void releaseAutoreleasePool(void* data);

	private:
		void enableDockIcon();

		UpdateDialogPrivate* d;
};

