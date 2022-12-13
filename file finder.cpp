#include <iostream>
#include <filesystem>
#include <string>
#include <conio.h>
#include <deque>
#include <thread>
#include <chrono>

using std::cout, std::string, std::deque, std::cin, std::this_thread::sleep_for, std::chrono::microseconds;
using namespace std::filesystem;
using namespace std::chrono;

/*
* written by chapel1337
* started on 12/11/2022
* finished on 12/12/2022
* there's not much to say about this, it's just a file finder
* tried to add a typewriter animation, but it took too long to write despite the delay being 2 milliseconds
* the release executable is apparently slower, so i added an extra 0 (it is now instant)
* the copy and delete feature are buggy; i'll fix them tomorrow
* i'm still releasing this despite the buggy behaviour because the main feature is functional and complete
* 
* 1>C:\Users\vvcr\OneDrive\Desktop\c++\file finder\file finder\file finder.cpp(345,59): warning C4390: ';': empty controlled statement found; is this the intent?
* 1>C:\Users\vvcr\OneDrive\Desktop\c++\file finder\file finder\file finder.cpp(360,59): warning C4390: ';': empty controlled statement found; is this the intent?
*/

deque<path> directoryContents{};

// path outputDirectory{ "C:\\Users\\vvcr\\OneDrive\\Desktop\\c++\\file finder\\Debug" };
path outputDirectory{};
path mainDirectory{};

int position{ 2 };
int action{};

string searchString{};

bool settingOutput{};
bool settingDirectory{};
bool searching{};
bool directorySet{};

void clear()
{
	system("cls");
}

// inefficient
void write(const string& input)
{
	for (const char& i : input)
	{
		cout << i;

		sleep_for(0.00005ms);
	}
}

void write(const string& input1, const string& input2)
{
	for (const char& i : input1)
	{
		cout << i;

		sleep_for(0.00005ms);
	}

	for (const char& i : input2)
	{
		cout << i;

		sleep_for(0.00005ms);
	}
}

void write(const string& input1, const string& input2, const string& input3)
{
	for (const char& i : input1)
	{
		cout << i;

		sleep_for(0.00005ms);
	}

	for (const char& i : input2)
	{
		cout << i;

		sleep_for(0.00005ms);
	}

	for (const char& i : input3)
	{
		cout << i;

		sleep_for(0.0005ms);
	}
}

void help()
{
	clear();

	cout << "\n	help\n\n";

	cout << "	this program is used for finding, copying, and deleting files in large directories\n";
	cout << "	note that when deleting or copying a directory, there will be no warning;\n";
	cout << "	be cautious when preforming these actions\n\n";

	cout << "	the main directory is the directory (including subdirectories) that are searched\n";
	cout << "	the output directory is the directy where copied files are placed\n\n";

	cout << "	when this program starts, you are automatically typing\n";
	cout << "	use the backspace key to remove the characters that have been typed into the search bar\n";
	cout << "	to return to this menu, press esc + h\n\n";

	cout << "	to set a main directory, press esc + m\n";
	cout << "	to set an output directory, press esc + o\n\n";

	cout << "	when setting these directories, a pointer will be present\n";
	cout << "	press w to move the pointer upwards, and press s to move the pointer downwards\n";
	cout << "	once you are in the desired directory, select 'set directory as ...' using the enter key\n\n";

	cout << "	when copying files, all of the files that were filtered in the search bar are copied to the output directory\n";
	cout << "	when deleting files, the same applies; the files will be deleted instead\n";

	cout << "	written by chapel1337\n";
	cout << "	started on 12/11/2022, finished on 12/12/2022\n\n";

	cout << "	okay	<\n";
	_getch();
}

void setDirectory(const path newDirectory)
{
	directoryContents.clear();
	position = 0;

	for (path i : recursive_directory_iterator(newDirectory))
	{
		if (i.parent_path() == mainDirectory)
		{
			directoryContents.push_front(i);
		}
		else
		{
			directoryContents.push_back(i);
		}
	}
	directoryContents.push_front(newDirectory / "..");

	if (settingOutput)
	{
		directoryContents.push_back(newDirectory / "set directory as output");
	}
	else if (settingDirectory)
	{
		directoryContents.push_back(newDirectory / "set directory as main");
	}

	directorySet = true;
}

void refresh()
{
	clear();

	if (directorySet)
	{
		write("	current directory:	", directoryContents[0].parent_path().string(), "\n\n");

		write("	main directory:		", mainDirectory.string(), "\n");
		write("	output directory:	", outputDirectory.string(), "\n\n\n");

		write("	>	", searchString, "	<\n\n");
	}
	else
	{
		cout << "	current directory:	" << directoryContents[0].parent_path().string() << "\n\n";

		cout << "	main directory:		" << mainDirectory.string() << '\n';
		cout << "	output directory:	" << outputDirectory.string() << "\n\n\n";

		cout << "	>	" << searchString << "	<\n\n";
	}

	if (action == 1)
	{
		write("	successfully copied directory to output folder");
		
		sleep_for(2s);

		action = 0;
		refresh();
	}
	else if (action == 2)
	{
		write("	successfully removed directory");

		sleep_for(2s);

		action = 0;
		refresh();
	}

	for (unsigned int i{}; i < directoryContents.size(); ++i)
	{
		if (i == 15 && searchString.empty())
		{
			cout << "	. . .\n";

			break;
		}

		if (settingOutput || settingDirectory)
		{
			if (!is_directory(directoryContents[i]) && i != 0 && i != directoryContents.size() - 1)
			{
				continue;
			}
			
			if (i == directoryContents.size() - 1)
			{
				cout << '\n';
			}

			if (directorySet)
			{
				write(directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length()));
			}
			else
			{
				directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length());
			}

			if (is_directory(directoryContents[i]))
			{
				cout << '\\';
			}
			if (i == position)
			{
				cout << "	<";
			}
		}
		else
		{
			if (i != 0 && directoryContents[i].stem().string().find(searchString) == string::npos && directoryContents[i].extension().string().find(searchString) == string::npos)
			{
				continue;
			}
			else
			{
				if (directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length()).length() > 100)
				{
					if (directorySet)
					{
						write("	", directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length()).substr(0, 100), " ...");
					}
					else
					{
						cout << "	" << directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length()).substr(0, 100) << " ...";
					}
				}
				else
				{
					if (directorySet)
					{
						write("	", directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length()));
					}
					else
					{
						cout << "	" << directoryContents[i].string().substr(mainDirectory.string().length() + 1, directoryContents[i].string().length());
					}
				}

				if (is_directory(directoryContents[i]))
				{
					cout << '\\';
				}
			}
		}

		cout << '\n';
	}

	directorySet = false;
}

void getInput()
{
	const int keyInput{ _getch() };

	if (keyInput == 27)
	{
		if (settingOutput)
		{
			settingOutput = false;

			setDirectory(mainDirectory);
		}
		else if (settingDirectory)
		{
			settingDirectory = false;

			setDirectory(mainDirectory);
		}
		else
		{
			int secondKeyInput{ _getch() };

			if ((secondKeyInput == 'O' || secondKeyInput == 'o') && !settingDirectory)
			{
				searchString.clear();
				searchString = "";

				if (!settingOutput)
				{
					settingOutput = true;
				}
				else
				{
					settingOutput = false;
				}

				setDirectory(directoryContents[0].parent_path());
				outputDirectory = directoryContents[0].parent_path();
			}
			else if ((secondKeyInput == 'M' || secondKeyInput == 'm') && !settingOutput)
			{
				searchString.clear();
				searchString = "";

				if (!settingDirectory)
				{
					settingDirectory = true;
				}
				else
				{
					settingDirectory = false;
				}

				setDirectory(directoryContents[0].parent_path());
				mainDirectory = directoryContents[0].parent_path();
			}
			else if ((secondKeyInput == 'C' || secondKeyInput == 'c') && !outputDirectory.empty())
			{
				for (unsigned int i{}; i < directoryContents.size(); ++i)
				{
					if (directoryContents[i].string().find(searchString));
					{
						copy(directoryContents[i], outputDirectory);
					}
				}

				searchString.clear();
				searchString = "";

				action = 1;
			}
			else if ((secondKeyInput == 'D' || secondKeyInput == 'd'))
			{
				for (unsigned int i{}; i < directoryContents.size(); ++i)
				{
					if (directoryContents[i].string().find(searchString));
					{
						remove(directoryContents[i]);
					}
				}

				setDirectory(mainDirectory);

				searchString.clear();
				searchString = "";

				action = 2;
			}
			else if ((secondKeyInput == 'H' || secondKeyInput == 'h'))
			{
				help();
			}
		}
	}
	if (settingDirectory || settingOutput)
	{
		if ((keyInput == 'W' || keyInput == 'w') && position != 0)
		{
			for (unsigned int i{ directoryContents.size() - 1 }; i > 0; --i)
			{
				if (i == directoryContents.size() - 1)
				{
					position = i;
				}

				if (is_directory(directoryContents[i]))
				{
					position = i;

					break;
				}
			}
		}
		else if ((keyInput == 'S' || keyInput == 's') && position != directoryContents.size() - 1)
		{
			for (int i{ position + 1 }; i < directoryContents.size(); ++i)
			{
				if (i == directoryContents.size() - 1)
				{
					position = i;
				}

				if (is_directory(directoryContents[i]))
				{
					position = i;

					break;
				}
			}
		}
		else if (keyInput == 13 || keyInput == ' ')
		{
			if (position == directoryContents.size() - 1)
			{
				if (settingOutput)
				{
					outputDirectory = directoryContents[0].parent_path();
					settingOutput = false;
				}
				else if (settingDirectory)
				{
					mainDirectory = directoryContents[0].parent_path();
					settingDirectory = false;
				}

				setDirectory(mainDirectory);
			}

			if (position == 0)
			{
				setDirectory(directoryContents[position].parent_path().parent_path());
			}
			else if (is_directory(directoryContents[position]))
			{
				setDirectory(directoryContents[position]);
			}
		}
	}
	else if (keyInput == 8 && !settingOutput && !settingDirectory && !searchString.empty())
	{
		searchString.pop_back();
	}
	else if (keyInput > 31 && keyInput != 127)
	{
		searchString.push_back(keyInput);
	}

	refresh();
	getInput();
}

int main()
{
	mainDirectory = current_path();

	setDirectory(mainDirectory);
	refresh();
	getInput();
}