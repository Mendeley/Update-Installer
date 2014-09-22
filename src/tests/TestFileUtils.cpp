#include "TestFileUtils.h"

#include "FileUtils.h"
#include "TestUtils.h"

void TestFileUtils::testDirName()
{
#ifdef PLATFORM_WINDOWS
	std::string dirName = FileUtils::dirname("E:/Some Dir/App.exe");
	TEST_COMPARE(dirName,"E:/Some Dir/");
#endif
}

void TestFileUtils::testIsRelative()
{
#ifdef PLATFORM_WINDOWS
	TEST_COMPARE(FileUtils::isRelative("temp"),true);
	TEST_COMPARE(FileUtils::isRelative("D:/temp"),false);
	TEST_COMPARE(FileUtils::isRelative("d:/temp"),false);
#else
	TEST_COMPARE(FileUtils::isRelative("/tmp"),false);
	TEST_COMPARE(FileUtils::isRelative("tmp"),true);
#endif
}

void TestFileUtils::testSymlinkFileExists()
{
#ifdef PLATFORM_UNIX
	const char* linkName = "link-name";
	FileUtils::removeFile(linkName);
	FileUtils::createSymLink(linkName, "target-that-does-not-exist");
	TEST_COMPARE(FileUtils::fileExists(linkName), true);
#endif
}

void TestFileUtils::testStandardDirs()
{
	std::string tmpDir = FileUtils::tempPath();
	TEST_COMPARE(FileUtils::fileExists(tmpDir.data()), true);
}

void TestFileUtils::testRemoveEmptyDirs()
{
	std::string tmpDir = FileUtils::tempPath();
	std::string rootDir = tmpDir + "/TestFileUtils-testRemoveEmptyDirs";
	std::string content = "non-empty-file-content";

	FileUtils::mkpath((rootDir + "/nested/empty/dir").c_str());
	FileUtils::mkpath((rootDir + "/nested/empty2/dir").c_str());
	FileUtils::writeFile((rootDir + "/nonempty.txt").c_str(), content.c_str(), content.size());
	FileUtils::removeEmptyDirs(rootDir.c_str());

	// root dir and the regular file should still exist
	TEST_COMPARE(FileUtils::fileExists(rootDir.c_str()), true);
	TEST_COMPARE(FileUtils::fileExists((rootDir + "/nonempty.txt").c_str()), true);

	// the empty nested directories should have been removed
	TEST_COMPARE(FileUtils::fileExists((rootDir + "/nested").c_str()), true);
}

int main(int,char**)
{
	TestList<TestFileUtils> tests;
	tests.addTest(&TestFileUtils::testDirName);
	tests.addTest(&TestFileUtils::testIsRelative);
	tests.addTest(&TestFileUtils::testSymlinkFileExists);
	tests.addTest(&TestFileUtils::testStandardDirs);
	return TestUtils::runTest(tests);
}
