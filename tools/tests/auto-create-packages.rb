#!/usr/bin/ruby
require 'fileutils.rb'
require 'find'
require 'optparse'
######################################## 
# copy 'src' to 'dest', preserving the attributes
# of 'src' 
def copy_file(src, dest)
	FileUtils.cp src, dest, :preserve => true
end

# Returns true if |src_file| and |dest_file| have the same contents, type
# and permissions or false otherwise
def compare_files(src_file, dest_file)
	if File.ftype(src_file) != File.ftype(dest_file)
		$stderr.puts "Type of file #{src_file} and #{dest_file} differ"
		return false
	end

	if File.file?(src_file) && !FileUtils.identical?(src_file, dest_file)
		$stderr.puts "Contents of file #{src_file} and #{dest_file} differ"
		return false
	end

	src_stat = File.stat(src_file)
	dest_stat = File.stat(dest_file)

	if src_stat.mode != dest_stat.mode
		$stderr.puts "Permissions of #{src_file} and #{dest_file} differ"
		return false
	end

	return true
end


# Compares the contents of two directories and returns a map of (file path => change type)
# for files and directories which differ between the two
def compare_dirs(src_dir, dest_dir)
	src_dir += '/' if !src_dir.end_with?('/')
	dest_dir += '/' if !dest_dir.end_with?('/')

	src_file_map = {}
	Find.find(src_dir) do |src_file|
		src_file = src_file[src_dir.length..-1]
		src_file_map[src_file] = nil
	end

	change_map = {}
	Find.find(dest_dir) do |dest_file|
		dest_file = dest_file[dest_dir.length..-1]

		if !src_file_map.include?(dest_file)
			change_map[dest_file] = :deleted
		elsif !compare_files("#{src_dir}/#{dest_file}", "#{dest_dir}/#{dest_file}")
			change_map[dest_file] = :updated
		end

		src_file_map.delete(dest_file)
	end

	src_file_map.each do |file, val|
		change_map[file] = :added
	end

	return change_map
end

def create_new_package(src_dir, dest_dir, package_dir)
	deleted_files = []
	change_map = compare_dirs(src_dir, dest_dir)
	change_map.each do |file, type|
		puts "#{file}, #{type}"
		if(type == :added || type == :updated)
			#puts "added...........updated"
			copy_file("#{src_dir}/#{file}", package_dir)
		elsif(type == :deleted)
			deleted_files << file;
		end
	end
	return deleted_files
end

def create_file(name, content)
	File.open(name, 'w') do |file|
		file.puts content
	end
	return name
end
puts "*********************"

###########
target_version = nil
old_version = "x.x.x"
OptionParser.new do |parser|
	parser.banner = "#{$0} <input old dir> <input new dir> <output dir> -v <version> [-o <old version>]"
	parser.on("-v","--version [version]","Specifies the target version string for this update") do |version|
		target_version = version
	end
	parser.on("-o","--oldversion [version]","Specifies the old version string for this update") do |oldversion|
		old_version = oldversion
	end
end.parse!

raise "Target version not specified (use -v option)" if !target_version

if ARGV.length < 3
	raise "Missing arguments ( #{$0} <input old dir> <input new dir> <output dir> -v version ) "
end

input_old_dir = ARGV[0]
input_new_dir = ARGV[1]
output_dir = ARGV[2]


dif_version_files = "temp"

# Remove the temp package dirs if they
# already exist
FileUtils.rm_rf(dif_version_files)
FileUtils.rm_rf(output_dir)
# Create the install directory with the old app
Dir.mkdir(dif_version_files)
#Dir.mkdir(output_dir)

# Create Version file
create_file("#{dif_version_files}/version.ini", target_version)

deleted_files = create_new_package(input_new_dir, input_old_dir, dif_version_files)

###########
puts "*********************"
###########

ENV["updater"] = "updater.exe"
ENV["dependency"] = "libbz2.dll"

ENV["platform"] = "MsWin32"
ENV["version"] = target_version

ENV["input"] = dif_version_files
ENV["package"] = "config.js"
ENV["output"] = output_dir
ENV["uninstall"] = deleted_files.join(" *spliter* ")

require_relative ("create-packages.rb")

# Remove the temp package dir
FileUtils.rm_rf(dif_version_files)

# get the all of each output files path
output_file_list = []
output_file_list_for_delete = []
output_zip_file = "#{old_version}_#{target_version}.zip"

Find.find(output_dir) do |path|
	next if (File.directory?(path) && !File.symlink?(path))
	output_file_list << "\"#{strip_prefix_slash(strip_prefix(path, output_dir))}\""
	output_file_list_for_delete << path
end

currentDir = Dir.pwd
Dir.chdir(output_dir) do
	if (!system("#{currentDir}/zip #{output_zip_file} #{output_file_list.join(" ")}"))
		raise "Failed to generate package zip file"
	else
		puts "Generated package zip file : #{output_zip_file}"
	end
end

output_file_list_for_delete.each do |file|
	FileUtils.rm_rf(file)
end
