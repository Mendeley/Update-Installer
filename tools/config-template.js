{
	// This file is the input to create-packages.rb. Its name ends in ".js" to
	// look better in syntax highlighting, because normally JSON files don't
	// contain comments (but Ruby's JSON parser allows them).

	"packages" : {
		// a single package which contains all files for the application
		"app" : [
			// Regular expression that will be applied to match which files
			// should this package contain.
			".*"
		]
	},
	
	// the base file name of the updater binary - this will be listed as
	// a dependency of the update process
	"updater-binary" : "updater",

	// The name of the main binary to launch to start the application. Relative
	// path from the input directory, so it could be "Contents/MacOS/myapp" on
	// OS X if you use a bundle, or "bin/myapp" if follows FHS conventions.
	"main-binary" : "myapp"
}
