// TextTranslationProgram.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

//A function that checks if the loaded element is a letter

bool letterWords(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

int main() {

	//Opening input and output files 

	std::ifstream text_file("Text.txt");
	std::ifstream TwoForeign_file("DictionaryTwoForeign.txt");
	std::ifstream OneForeign_file("DictionaryOneForeign.txt");
	std::ifstream WithoutTranslation_file("WordsWithoutTranslation.txt");
	std::ofstream translation_file("Translation.txt");

	//Defining a map for words from a dictionary

	std::map<std::string, std::string>dictionary;

	std::vector<std::string>string_FirstWord;
	std::vector<std::string>string_ComplexWord;

	//Loading complex words from the dictionary

	while (true) {
		std::string word, first_foreign, second_foreign, translation;

		TwoForeign_file >> first_foreign >> second_foreign >> translation;
		if (TwoForeign_file) {

			word = first_foreign + " " + second_foreign;
			string_FirstWord.push_back(first_foreign);
			string_ComplexWord.push_back(word);
			dictionary[word] = translation;
		}
		else {
			break;
		}
	}

	std::vector<std::string>string_OneForeign;

	//Loading simple words from the dictionary

	while (true) {
		std::string word, translation;
		OneForeign_file >> word >> translation;
		if (OneForeign_file) {
			string_OneForeign.push_back(word);
			dictionary[word] = translation;
		}
		else { break; }
	}

	std::vector<std::string>string_UntranslatedWord;

	//Loading words from the dictionary which are untranslatable from English to Serbian, 
	//such as definite and indefinite articles etc.

	while (true) {
		std::string word;
		WithoutTranslation_file >> word;
		if (WithoutTranslation_file) { string_UntranslatedWord.push_back(word); }
		else { break; }
	}

	std::string loaded_word;
	std::string first_loaded;
	std::string compound_word;

	//Loading words from the text

	while (true) {
		char c1 = text_file.get();      //Loading letters, characters and spaces from the text  
		if (text_file) {
			if (letterWords(c1)) {      //Checking if the loaded element is a letter
				loaded_word += c1;
			}

			else {

				compound_word = first_loaded + " " + loaded_word;        //Making a word that consists of the penultimate and the last loaded word
																		 //to check whether that word is a complex word from the dictionary
				int indicatorComplex = 0;
				for (std::string complexWord : string_ComplexWord) {
					if (compound_word == complexWord) {
						indicatorComplex = 1;                            //Setting the indicator to 1 if the complex word is in the dictionary
					}
				}

				int indicatorUntranslated = 0;
				for (std::string untranslatedWord : string_UntranslatedWord) {
					if (loaded_word == untranslatedWord) {
						loaded_word = "";
						indicatorUntranslated = 1;                        //Setting the indicator to 1 if the loaded word is the untranslatable word
					}
				}

				int indicatorFirst = 0;
				for (std::string wordFirst : string_FirstWord) {
					if (loaded_word == wordFirst) {
						indicatorFirst = 1;                               //Setting the indicator to 1 if the loaded word is the first word from the dictionary of complex words
					}
				}


				int indicatorOnly = 0;
				for (std::string onlyWord : string_OneForeign) {
					if (loaded_word == onlyWord) {
						indicatorOnly = 1;                                //Setting the indicator to 1 if the loaded word is in the dictionary of simple words
					}
				}


				//*********************************************//
				//Checking if the loaded word is both in the dictionary of complex words and in the dictionary of simple words and whether that word will be a part of a complex word
				//*********************************************//


				if (indicatorFirst == 1 && indicatorOnly == 1) {
					std::string loaded_second;
					std::string potencial_complex;      //a string that checks if the loaded word will be complex

					char c2;
					while (true) {
						c2 = text_file.get();
						if (letterWords(c2)) {
							loaded_second += c2;
						}
						else {
							break;
						}
					}

					potencial_complex = loaded_word + " " + loaded_second;

					int indicatorComplex = 0;
					for (std::string complexWord : string_ComplexWord) {
						if (potencial_complex == complexWord) {
							indicatorComplex = 1;                          //Setting the indicator to 1 if the word is complex
						}
					}

					if (indicatorComplex == 1) {
						translation_file << dictionary[potencial_complex] << c2;     //If the word is complex, typing in the translation of that word in translation_file
						loaded_word = "";
					}
					else {
						translation_file << dictionary[loaded_word] << c1 << dictionary[loaded_second] << c2;   //If the word is not complex, typing in the translation  
																												//of that loaded simple words in translation_file
						loaded_word = "";                                                                       
					}
				}

				//If the loaded word is neither in the dictionary of complex words nor in the dictionary of simple words, typing in the following cases

				else {
					if (indicatorUntranslated == 1 || indicatorFirst == 1) {      //Checking if the loaded word is translatable or if it is a part of a complex word
						if (indicatorUntranslated < indicatorFirst) {
							first_loaded = loaded_word;                           //If the loaded word is the first word in the dictionary of complex words, then memorizing the loaded word in the string first_loaded
							loaded_word = "";
						}
					}

					else {
						if (indicatorComplex != 1) {

							if (indicatorOnly == 0) {                       //If the loaded word is not in the dictionary of simple words, then just typing in that 
								translation_file << loaded_word << c1;      //word without translating it in translation_file
								loaded_word = "";
							}
							else {
								translation_file << dictionary[loaded_word] << c1;     //Typing in the translation of simple words in translation_file                             													  
								loaded_word = "";
							}
						}
						else {
							translation_file << dictionary[compound_word] << c1;       //Typing in the translation of complex words in translation_file
							loaded_word = "";
						}
					}
				}
			}
		}
		else { break; }
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
