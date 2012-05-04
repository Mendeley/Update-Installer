#pragma once

#include <sigslot.h>

/** Controller used by the UI to drive the installation
  * and monitor the status of the install.
  */
class UpdateController
{
	public:
		UpdateController();

		/** Emitted to indicate progress of update installation. */
		sigslot::signal1<int> installProgress;

		/** Emitted when installation fails for any reason. */
		sigslot::signal1<const std::string&> installError;

		/** Emitted when installation completes, whether successfully or
		  * following an unrecoverable error.
		  */
		sigslot::signal0<> finished;
};
