// 2015/12/08 Junjie Chen
#include "RhymeBot.h"

RhymeBot::RhymeBot(){
	toQuery = "";
	for(int i=0; i < 4; i++){
		partOfSpeechVector.push_back(new multimap<int, string>());
		headProVector.push_back(new multimap<int, multimap<string, string>>());
		tailProVector.push_back(new multimap<int, multimap<string, string>>());

		counterVector.push_back(new set<int>());
		headProTwoCounterVector.push_back(new set<int>());
		headProThreeCounterVector.push_back(new set<int>());
		tailProTwoCounterVector.push_back(new set<int>());
		tailProThreeCounterVector.push_back(new set<int>());

		headProTwoVector.push_back(new multimap<int, multimap<string, string>>());
		headProThreeVector.push_back(new multimap<int, multimap<string, string>>());
		tailProTwoVector.push_back(new multimap<int, multimap<string, string>>());
		tailProThreeVector.push_back(new multimap<int, multimap<string, string>>());
		superHasher.push_back(new multimap<int, multimap<long long, string>>());
		superHasherCounterVector.push_back(new set<int>());
	}
}

/** Inserting Words **/

void RhymeBot::insertWithPronunciation(string word, vector<Sound> pronunciation){
	//	cout << "INSERT\n";
	dictionaryMap.insert(pair<string, vector<Sound>>(word, pronunciation));
}

void RhymeBot::addPartOfSpeech(string word, PART_OF_SPEECH partOfSpeech){
	if(dictionaryMap.find(word) != dictionaryMap.end()){
		setPartOfSpeechVector(word, partOfSpeech);
	}
}


/** Helper Functions **/

// returns the number of syllables in word
int RhymeBot::countSyllables(string word){
	int counter = 0;
	if(dictionaryMap.find(word) != dictionaryMap.end()){
		for(unsigned int i=0; i < dictionaryMap.find(word)->second.size(); i++){
			if(dictionaryMap.find(word)->second.at(i).isVowel()){
				counter++;
			}
		}
	}
	return counter;
}

// returns the number of rhyming syllables in the two words. This is defined as the number of vowel sounds in the
//  largest rhyming suffix.
//
// Words are considered a rhyme if they have the same last vowel sound as well as the same sounds following
//  the last vowel. Note: These are syllabic rhymes since the matching vowels might not both be stressed
int RhymeBot::rhymingSyllables(string word1, string word2){
	int counter = 0;
	if(dictionaryMap.find(word1) != dictionaryMap.end() && dictionaryMap.find(word2) != dictionaryMap.end()){
		vector<Sound>::iterator itOne = (dictionaryMap.find(word1)->second.end())-1;
		vector<Sound>::iterator itTwo = (dictionaryMap.find(word2)->second.end())-1;
		while(true){
			if(*itOne != *itTwo){
				return counter;
			}
			else{
				if((*itOne).isVowel() && *itOne == *itTwo){
					counter++;
				}
			}
			if(itOne == dictionaryMap.find(word1)->second.begin() || itTwo == dictionaryMap.find(word2)->second.begin()){
				return counter;
			}
			else{
				itOne--;
				itTwo--;
			}
		}
	}
	return counter;
}

// returns the number of identical sounds in the largest matching prefix of the two words.
//
// Words are considered an alliteration if they have the same first sound
int RhymeBot::alliterationSounds(string word1, string word2){
	int counter = 0;
	if(dictionaryMap.find(word1) != dictionaryMap.end() && dictionaryMap.find(word2) != dictionaryMap.end()){
		vector<Sound>::iterator itOne = dictionaryMap.find(word1)->second.begin();
		vector<Sound>::iterator itTwo = dictionaryMap.find(word2)->second.begin();
		while(true){
			if(*itOne != *itTwo){
				return counter;
			}
			else{
				counter++;
			}

			if(itOne == (dictionaryMap.find(word1)->second.end())-1 || itTwo == (dictionaryMap.find(word2)->second.end())-1){
				return counter;
			}
			else{
				itOne++;
				itTwo++;
			}
		}
	}
	return counter;
}

/** Search for Rhymes and Alliterations **/

// returns all words that rhyme at least 1 syllable with the input word
// returns the empty vector if no rhymes are found or if the given word is not in the dictionary
vector<string>* RhymeBot::getAllRhymes(string word){
	vector<string>* rhymingWords = new vector<string>;
	if(dictionaryMap.find(word) == dictionaryMap.end()){
		cout << "word not found!\n";
		return rhymingWords;
	}
	else{
		for(map<string, vector<Sound>>::iterator it = dictionaryMap.begin(); it != dictionaryMap.end(); it++){
			if(it->first != word && rhymingSyllables(word, it->first) > 0){
				rhymingWords->push_back(it->first);
			}
		}
	}
	return rhymingWords;
}

// returns all words that form an alliteration of at least 1 sound with the input word
// returns the empty vector if no rhymes are found or the given word is not known
vector<string>* RhymeBot::getAllAlliterations(string word){
	vector<string>* alliterationWords = new vector<string>;
	if(dictionaryMap.find(word) == dictionaryMap.end()){
		cout << "word not found!\n";
		return alliterationWords;
	}
	else{
		for(map<string, vector<Sound>>::iterator it = dictionaryMap.begin(); it != dictionaryMap.end(); it++){
			if(it->first != word && alliterationSounds(word, it->first) > 0){
				alliterationWords->push_back(it->first);
			}
		}
	}
	return alliterationWords;
}

/** Finding the Perfect Word **/

// returns the best word with the specified number of syllables and part of speech that rhymes with the given word.
// Best is defined by the maximum number of rhyming syllables.
string RhymeBot::findBestRhyme(int numberOfSyllables, string rhymesWith){
	string bestMatch = "";
	int maxRhyme = 0;
	if(dictionaryMap.find(rhymesWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		for(map<string, vector<Sound>>::iterator it = dictionaryMap.begin(); it != dictionaryMap.end(); it++){
			if(countSyllables(it->first) == numberOfSyllables){
				if(rhymesWith != it->first && rhymingSyllables(rhymesWith, it->first) > maxRhyme){
					maxRhyme = rhymingSyllables(rhymesWith, it->first);
					bestMatch = it->first;
				}
			}
		}
	}
	return bestMatch;
}
string RhymeBot::findBestRhyme(PART_OF_SPEECH partOfSpeech, string rhymesWith){
	string bestMatch = "";
	int maxRhyme = 0;
	if(dictionaryMap.find(rhymesWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		multimap<int, string> tempMap = getPartOfSpeechMap(partOfSpeech);
		for(multimap<int, string>::iterator it = tempMap.begin(); it != tempMap.end(); it++){
			if(rhymesWith != it->second && rhymingSyllables(rhymesWith, it->second) > maxRhyme){
				maxRhyme = rhymingSyllables(rhymesWith, it->second);
				bestMatch = it->second;
			}
		}
	}
	return bestMatch;
}
string RhymeBot::findBestRhyme(int numberOfSyllables, PART_OF_SPEECH partOfSpeech, string rhymesWith){
	string bestMatch = "";
	int maxRhyme = 0;
	if(dictionaryMap.find(rhymesWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		multimap<string, string> tempMap = getTailProMap(partOfSpeech, numberOfSyllables, rhymesWith);
//		cout << "USE " << toQuery << " to query tail for " << rhymesWith << endl;
		pair<multimap<string, string>::iterator, multimap<string, string>::iterator> range = tempMap.equal_range(toQuery);
		for(multimap<string, string>::iterator it = range.first; it != range.second; it++){
			if(rhymesWith != it->second && rhymingSyllables(rhymesWith, it->second) > maxRhyme){
				maxRhyme = rhymingSyllables(rhymesWith, it->second);
				bestMatch = it->second;
			}
		}
	}
	return bestMatch;
}

// returns the best word with the specified number of syllables and part of speech that forms an alliteration with
// the given word. Best is defined by the number of maximum number of alliterating sounds.
string RhymeBot::findBestAlliteration(int numberOfSyllables, string alliterationWith){
	string bestMatch = "";
	int maxAlliteration = 0;
	if(dictionaryMap.find(alliterationWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		for(map<string, vector<Sound>>::iterator it = dictionaryMap.begin(); it != dictionaryMap.end(); it++){
			if(countSyllables(it->first) == numberOfSyllables){
				if(alliterationWith != it->first && alliterationSounds(alliterationWith, it->first) > maxAlliteration){
					maxAlliteration = alliterationSounds(alliterationWith, it->first);
					bestMatch = it->first;
				}
			}
		}
	}
	return bestMatch;
}
string RhymeBot::findBestAlliteration(PART_OF_SPEECH partOfSpeech, string alliterationWith){
	string bestMatch = "";
	int maxAlliteration = 0;
	if(dictionaryMap.find(alliterationWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		multimap<int, string> tempMap = getPartOfSpeechMap(partOfSpeech);
		for(map<int, string>::iterator it = tempMap.begin(); it != tempMap.end(); it++){
			if(alliterationWith != it->second && alliterationSounds(alliterationWith, it->second) > maxAlliteration){
				maxAlliteration = alliterationSounds(alliterationWith, it->second);
				bestMatch = it->second;
			}
		}
	}
	return bestMatch;
}
string RhymeBot::findBestAlliteration(int numberOfSyllables, PART_OF_SPEECH partOfSpeech, string alliterationWith){
	string bestMatch = "";
	int maxAlliteration = 0;
	if(dictionaryMap.find(alliterationWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		multimap<string, string> tempMap = getHeadProMap(partOfSpeech, numberOfSyllables, alliterationWith);
//		cout << "USE " << toQuery << " to query head for " << alliterationWith << endl;
		pair<multimap<string, string>::iterator, multimap<string, string>::iterator> range = tempMap.equal_range(toQuery);
		for(map<string, string>::iterator it = range.first; it != range.second; it++){

			if(alliterationWith != it->second && alliterationSounds(alliterationWith, it->second) > maxAlliteration){
				maxAlliteration = alliterationSounds(alliterationWith, it->second);
				bestMatch = it->second;
			}
		}
	}
	return bestMatch;
}

// Combine findBestRhyme and findBestAlliteration. Best is defined by the sum of the number of rhyming syllables
//  and the number of alliterating sounds.
string RhymeBot::findPerfectWord(int numberOfSyllables, PART_OF_SPEECH partOfSpeech, string rhymesWith, string alliterationWith){
	string bestMatch = "";
	int sum = 0;
	if(dictionaryMap.find(alliterationWith) == dictionaryMap.end()){
		return bestMatch;
	}
	else{
		bestMatch = theBest(numberOfSyllables, partOfSpeech, rhymesWith, alliterationWith);
	}
	return bestMatch;
}

int RhymeBot::dictSize(){
	return dictionaryMap.size();
}

int RhymeBot::partOfSpeechVectorSize(){
	int size = 0;
	for(unsigned int i = 0; i<partOfSpeechVector.size(); i++){
		size += partOfSpeechVector.at(i)->size();
	}
	return size;
}

void RhymeBot::setPartOfSpeechVector(string word, PART_OF_SPEECH partOfSpeech){
	//	cout << "INTO POS " << word << endl;
	PART_OF_SPEECH pos = partOfSpeech;
	string headProOne = "" +  dictionaryMap.find(word)->second.at(0).getSourceString();
	string tailProOne = "" + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-1).getSourceString();
	string headProTwo = "";
	string tailProTwo = "";
	string headProThree = "";
	string tailProThree = "";
	string headAndTail = "" + headProOne + tailProOne;
	if(dictionaryMap.find(word)->second.size() >= 2){
		headProTwo = "" + headProOne +  dictionaryMap.find(word)->second.at(1).getSourceString();
		//		cout << "headProTwo of " << word << " is " << headProTwo << endl;
		tailProTwo = "" + tailProOne + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-2).getSourceString();
		//		cout << "tailProTwo of " << word << " is " << tailProTwo << endl;
	}

	if(dictionaryMap.find(word)->second.size() >= 3){
		headProThree = "" + headProTwo +  dictionaryMap.find(word)->second.at(2).getSourceString();
		//		cout << "headProThree of " << word << " is " << headProThree << endl;
		tailProThree = "" + tailProTwo + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-3).getSourceString();
		//		cout << "tailProThree of " << word << " is " << tailProThree << endl;
	}

	int nos = countSyllables(word);
	//	cout << word << ", nos " << nos << endl;
	int index;
	switch(pos){
	case ADJECTIVE:
		index = 0;
		partOfSpeechVector.at(index)->insert(pair<int, string>(countSyllables(word), word));
		// pro 1
		if(counterVector.at(index)->find(nos) == counterVector.at(index)->end()){
			counterVector.at(index)->insert(nos);
			multimap<string, string> headMap;
			multimap<string, string> tailMap;
			headMap.insert(make_pair(headProOne, word));
			tailMap.insert(make_pair(tailProOne, word));
			headProVector.at(index)->insert(make_pair(nos, headMap));
			tailProVector.at(index)->insert(make_pair(nos, tailMap));
		}
		else{
			multimap<string, string> & headMapRef = headProVector.at(index)->find(nos)->second;
			multimap<string, string> & tailMapRef = tailProVector.at(index)->find(nos)->second;
			headMapRef.insert(make_pair(headProOne, word));
			tailMapRef.insert(make_pair(tailProOne, word));
		}

		// pro 2
		if(headProTwoCounterVector.at(index)->find(nos) == headProTwoCounterVector.at(index)->end()){
			headProTwoCounterVector.at(index)->insert(nos);
			tailProTwoCounterVector.at(index)->insert(nos);
			multimap<string, string> headProTwoMap;
			multimap<string, string> tailProTwoMap;
			headProTwoMap.insert(make_pair(headProTwo, word));
			tailProTwoMap.insert(make_pair(tailProTwo, word));
			headProTwoVector.at(index)->insert(make_pair(nos, headProTwoMap));
			tailProTwoVector.at(index)->insert(make_pair(nos, tailProTwoMap));
		}
		else{
			multimap<string, string> & headProTwoMapRef = headProTwoVector.at(index)->find(nos)->second;
			multimap<string, string> & tailProTwoMapRef = tailProTwoVector.at(index)->find(nos)->second;
			headProTwoMapRef.insert(make_pair(headProTwo, word));
			tailProTwoMapRef.insert(make_pair(tailProTwo, word));
		}

		//		 pro 3
		if(headProThreeCounterVector.at(index)->find(nos) == headProThreeCounterVector.at(index)->end()){
			headProThreeCounterVector.at(index)->insert(nos);
			tailProThreeCounterVector.at(index)->insert(nos);
			multimap<string, string> headProThreeMap;
			multimap<string, string> tailProThreeMap;
			headProThreeMap.insert(make_pair(headProThree, word));
			tailProThreeMap.insert(make_pair(tailProThree, word));
			headProThreeVector.at(index)->insert(make_pair(nos, headProThreeMap));
			tailProThreeVector.at(index)->insert(make_pair(nos, tailProThreeMap));
		}
		else{
			multimap<string, string> & headProThreeMapRef = headProThreeVector.at(index)->find(nos)->second;
			multimap<string, string> & tailThreeMapRef = tailProThreeVector.at(index)->find(nos)->second;
			headProThreeMapRef.insert(make_pair(headProThree, word));
			tailThreeMapRef.insert(make_pair(tailProThree, word));
		}

		// hasher
		if(superHasherCounterVector.at(index)->find(nos) == superHasherCounterVector.at(index)->end()){
			superHasherCounterVector.at(index)->insert(nos);
			multimap<long long, string> hashTable;
			hashTable.insert(make_pair(hasher(headAndTail), word));
			superHasher.at(index)->insert(make_pair(nos, hashTable));
		}
		else{
			multimap<long long, string> & hashTableRef = superHasher.at(index)->find(nos)->second;
			hashTableRef.insert(make_pair(hasher(headAndTail), word));
		}
		//		cout << nos << ", " << headProTwoVector.at(index)->find(countSyllables(word))->second.size() << endl;
		//		cout << nos << ", " << tailProTwoVector.at(index)->find(countSyllables(word))->second.size() << endl;
		break;

	case ADVERB:
		//		cout << "ADV\n";
		index = 1;
		partOfSpeechVector.at(index)->insert(pair<int, string>(countSyllables(word), word));
		// pro 1
		if(counterVector.at(index)->find(nos) == counterVector.at(index)->end()){
			//			cout << "new pro 1 with nos " << nos << endl;
			counterVector.at(index)->insert(nos);
			multimap<string, string> headMap;
			multimap<string, string> tailMap;
			headMap.insert(make_pair(headProOne, word));
			tailMap.insert(make_pair(tailProOne, word));
			headProVector.at(index)->insert(make_pair(nos, headMap));
			tailProVector.at(index)->insert(make_pair(nos, tailMap));
		}
		else{
			//			cout << "old pro 1\n";
			multimap<string, string> & headMapRef = headProVector.at(index)->find(nos)->second;
			multimap<string, string> & tailMapRef = tailProVector.at(index)->find(nos)->second;
			headMapRef.insert(make_pair(headProOne, word));
			tailMapRef.insert(make_pair(tailProOne, word));
		}

		// pro 2
		if(headProTwoCounterVector.at(index)->find(nos) == headProTwoCounterVector.at(index)->end()){
			//			cout << "new pro 2 with nos " << nos << endl;
			headProTwoCounterVector.at(index)->insert(nos);
			tailProTwoCounterVector.at(index)->insert(nos);
			multimap<string, string> headProTwoMap;
			multimap<string, string> tailProTwoMap;
			headProTwoMap.insert(make_pair(headProTwo, word));
			tailProTwoMap.insert(make_pair(tailProTwo, word));
			headProTwoVector.at(index)->insert(make_pair(nos, headProTwoMap));
			tailProTwoVector.at(index)->insert(make_pair(nos, tailProTwoMap));
		}
		else{
			//			cout << "old pro 2\n";
			multimap<string, string> & headProTwoMapRef = headProTwoVector.at(index)->find(nos)->second;
			multimap<string, string> & tailTwoMapRef = tailProTwoVector.at(index)->find(nos)->second;
			headProTwoMapRef.insert(make_pair(headProTwo, word));
			tailTwoMapRef.insert(make_pair(tailProTwo, word));
		}

		//		 pro 3
		if(headProThreeCounterVector.at(index)->find(nos) == headProThreeCounterVector.at(index)->end()){
			//			cout << "new pro 3 with nos " << nos << endl;
			headProThreeCounterVector.at(index)->insert(nos);
			tailProThreeCounterVector.at(index)->insert(nos);
			multimap<string, string> headProThreeMap;
			multimap<string, string> tailProThreeMap;
			headProThreeMap.insert(make_pair(headProThree, word));
			tailProThreeMap.insert(make_pair(tailProThree, word));
			headProThreeVector.at(index)->insert(make_pair(nos, headProThreeMap));
			tailProThreeVector.at(index)->insert(make_pair(nos, tailProThreeMap));
		}
		else{
			//			cout << "old pro 3\n";
			multimap<string, string> & headProThreeMapRef = headProThreeVector.at(index)->find(nos)->second;
			multimap<string, string> & tailThreeMapRef = tailProThreeVector.at(index)->find(nos)->second;
			headProThreeMapRef.insert(make_pair(headProThree, word));
			tailThreeMapRef.insert(make_pair(tailProThree, word));
		}
		// hasher
		if(superHasherCounterVector.at(index)->find(nos) == superHasherCounterVector.at(index)->end()){
			superHasherCounterVector.at(index)->insert(nos);
			multimap<long long, string> hashTable;
			hashTable.insert(make_pair(hasher(headAndTail), word));
			superHasher.at(index)->insert(make_pair(nos, hashTable));
		}
		else{
			multimap<long long, string> & hashTableRef = superHasher.at(index)->find(nos)->second;
			hashTableRef.insert(make_pair(hasher(headAndTail), word));
		}
		//		cout << headProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		//		cout << tailProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		break;

	case NOUN:
		//		cout << "NOUN\n";
		index = 2;
		partOfSpeechVector.at(index)->insert(pair<int, string>(countSyllables(word), word));
		// pro 1
		if(counterVector.at(index)->find(nos) == counterVector.at(index)->end()){
			//			cout << "new pro 1 with nos " << nos << endl;
			counterVector.at(index)->insert(nos);
			multimap<string, string> headMap;
			multimap<string, string> tailMap;
			headMap.insert(make_pair(headProOne, word));
			tailMap.insert(make_pair(tailProOne, word));
			headProVector.at(index)->insert(make_pair(nos, headMap));
			tailProVector.at(index)->insert(make_pair(nos, tailMap));
		}
		else{
			//			cout << "old pro 1\n";
			multimap<string, string> & headMapRef = headProVector.at(index)->find(nos)->second;
			multimap<string, string> & tailMapRef = tailProVector.at(index)->find(nos)->second;
			headMapRef.insert(make_pair(headProOne, word));
			tailMapRef.insert(make_pair(tailProOne, word));
		}

		// pro 2
		if(headProTwoCounterVector.at(index)->find(nos) == headProTwoCounterVector.at(index)->end()){
			//			cout << "new pro 2 with nos " << nos << endl;
			headProTwoCounterVector.at(index)->insert(nos);
			tailProTwoCounterVector.at(index)->insert(nos);
			multimap<string, string> headProTwoMap;
			multimap<string, string> tailProTwoMap;
			headProTwoMap.insert(make_pair(headProTwo, word));
			tailProTwoMap.insert(make_pair(tailProTwo, word));
			headProTwoVector.at(index)->insert(make_pair(nos, headProTwoMap));
			tailProTwoVector.at(index)->insert(make_pair(nos, tailProTwoMap));
		}
		else{
			//			cout << "old pro 2\n";
			multimap<string, string> & headProTwoMapRef = headProTwoVector.at(index)->find(nos)->second;
			multimap<string, string> & tailTwoMapRef = tailProTwoVector.at(index)->find(nos)->second;
			headProTwoMapRef.insert(make_pair(headProTwo, word));
			tailTwoMapRef.insert(make_pair(tailProTwo, word));
		}

		//		 pro 3
		if(headProThreeCounterVector.at(index)->find(nos) == headProThreeCounterVector.at(index)->end()){
			//			cout << "new pro 3 with nos " << nos << endl;
			headProThreeCounterVector.at(index)->insert(nos);
			tailProThreeCounterVector.at(index)->insert(nos);
			multimap<string, string> headProThreeMap;
			multimap<string, string> tailProThreeMap;
			headProThreeMap.insert(make_pair(headProThree, word));
			tailProThreeMap.insert(make_pair(tailProThree, word));
			headProThreeVector.at(index)->insert(make_pair(nos, headProThreeMap));
			tailProThreeVector.at(index)->insert(make_pair(nos, tailProThreeMap));
		}
		else{
			//			cout << "old pro 3\n";
			multimap<string, string> & headProThreeMapRef = headProThreeVector.at(index)->find(nos)->second;
			multimap<string, string> & tailThreeMapRef = tailProThreeVector.at(index)->find(nos)->second;
			headProThreeMapRef.insert(make_pair(headProThree, word));
			tailThreeMapRef.insert(make_pair(tailProThree, word));
		}
		// hasher
		if(superHasherCounterVector.at(index)->find(nos) == superHasherCounterVector.at(index)->end()){
			superHasherCounterVector.at(index)->insert(nos);
			multimap<long long, string> hashTable;
			hashTable.insert(make_pair(hasher(headAndTail), word));
			superHasher.at(index)->insert(make_pair(nos, hashTable));
		}
		else{
			multimap<long long, string> & hashTableRef = superHasher.at(index)->find(nos)->second;
			hashTableRef.insert(make_pair(hasher(headAndTail), word));
		}
		//		cout << headProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		//		cout << tailProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		break;

	case VERB:
		//		cout << "VERB\n";
		index = 3;
		partOfSpeechVector.at(index)->insert(pair<int, string>(countSyllables(word), word));
		// pro 1
		if(counterVector.at(index)->find(nos) == counterVector.at(index)->end()){
			//			cout << "new pro 1 with nos " << nos << endl;
			counterVector.at(index)->insert(nos);
			multimap<string, string> headMap;
			multimap<string, string> tailMap;
			headMap.insert(make_pair(headProOne, word));
			tailMap.insert(make_pair(tailProOne, word));
			headProVector.at(index)->insert(make_pair(nos, headMap));
			tailProVector.at(index)->insert(make_pair(nos, tailMap));
		}
		else{
			//			cout << "old pro 1\n";
			multimap<string, string> & headMapRef = headProVector.at(index)->find(nos)->second;
			multimap<string, string> & tailMapRef = tailProVector.at(index)->find(nos)->second;
			headMapRef.insert(make_pair(headProOne, word));
			tailMapRef.insert(make_pair(tailProOne, word));
		}

		// pro 2
		if(headProTwoCounterVector.at(index)->find(nos) == headProTwoCounterVector.at(index)->end()){
			//			cout << "new pro 2 with nos " << nos << endl;
			headProTwoCounterVector.at(index)->insert(nos);
			tailProTwoCounterVector.at(index)->insert(nos);
			multimap<string, string> headProTwoMap;
			multimap<string, string> tailProTwoMap;
			headProTwoMap.insert(make_pair(headProTwo, word));
			tailProTwoMap.insert(make_pair(tailProTwo, word));
			headProTwoVector.at(index)->insert(make_pair(nos, headProTwoMap));
			tailProTwoVector.at(index)->insert(make_pair(nos, tailProTwoMap));
		}
		else{
			//			cout << "old pro 2\n";
			multimap<string, string> & headProTwoMapRef = headProTwoVector.at(index)->find(nos)->second;
			multimap<string, string> & tailTwoMapRef = tailProTwoVector.at(index)->find(nos)->second;
			headProTwoMapRef.insert(make_pair(headProTwo, word));
			tailTwoMapRef.insert(make_pair(tailProTwo, word));
		}

		//		 pro 3
		if(headProThreeCounterVector.at(index)->find(nos) == headProThreeCounterVector.at(index)->end()){
			//			cout << "new pro 3 with nos " << nos << endl;
			headProThreeCounterVector.at(index)->insert(nos);
			tailProThreeCounterVector.at(index)->insert(nos);
			multimap<string, string> headProThreeMap;
			multimap<string, string> tailProThreeMap;
			headProThreeMap.insert(make_pair(headProThree, word));
			tailProThreeMap.insert(make_pair(tailProThree, word));
			headProThreeVector.at(index)->insert(make_pair(nos, headProThreeMap));
			tailProThreeVector.at(index)->insert(make_pair(nos, tailProThreeMap));
		}
		else{
			//			cout << "old pro 3\n";
			multimap<string, string> & headProThreeMapRef = headProThreeVector.at(index)->find(nos)->second;
			multimap<string, string> & tailThreeMapRef = tailProThreeVector.at(index)->find(nos)->second;
			headProThreeMapRef.insert(make_pair(headProThree, word));
			tailThreeMapRef.insert(make_pair(tailProThree, word));
		}
		// hasher
		if(superHasherCounterVector.at(index)->find(nos) == superHasherCounterVector.at(index)->end()){
			superHasherCounterVector.at(index)->insert(nos);
			multimap<long long, string> hashTable;
			hashTable.insert(make_pair(hasher(headAndTail), word));
			superHasher.at(index)->insert(make_pair(nos, hashTable));
		}
		else{
			multimap<long long, string> & hashTableRef = superHasher.at(index)->find(nos)->second;
			hashTableRef.insert(make_pair(hasher(headAndTail), word));
		}
		//		cout << headProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		//		cout << tailProThreeVector.at(index)->find(countSyllables(word))->second.size() << endl;
		break;
	}
}

multimap<int, string>& RhymeBot::getPartOfSpeechMap(PART_OF_SPEECH partOfSpeech){
	PART_OF_SPEECH pos = partOfSpeech;
	switch(pos){
	case ADJECTIVE:
		//		cout << "Fetching ADJ map\n";
		return *(partOfSpeechVector.at(0));
		break;
	case ADVERB:
		//		cout << "Fetching ADV map\n";
		return *(partOfSpeechVector.at(1));
		break;
	case NOUN:
		//		cout << "Fetching NOUN map\n";
		return *(partOfSpeechVector.at(2));
		break;
	case VERB:
		//		cout << "Fetching VERB map\n";
		return *(partOfSpeechVector.at(3));
		break;
	}
}

multimap<string, string>& RhymeBot::getHeadProMap(PART_OF_SPEECH partOfSpeech, int numberOfSyllables, string word){
	PART_OF_SPEECH pos = partOfSpeech;
	int index;
	string headProOne = "" +  dictionaryMap.find(word)->second.at(0).getSourceString();
	string tailProOne = "" + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-1).getSourceString();
	string headProTwo = "";
	string tailProTwo = "";
	string headProThree = "";
	string tailProThree = "";
	if(dictionaryMap.find(word)->second.size() >= 2){
		headProTwo = "" + headProOne +  dictionaryMap.find(word)->second.at(1).getSourceString();
		tailProTwo = "" + tailProOne + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-2).getSourceString();
	}

	if(dictionaryMap.find(word)->second.size() >= 3){
		headProThree = "" + headProTwo +  dictionaryMap.find(word)->second.at(2).getSourceString();
		tailProThree = "" + tailProTwo + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-3).getSourceString();
	}
	switch(pos){
	case ADJECTIVE:
		index = 0;
		if(headProThreeVector.at(index)->find(numberOfSyllables)->second.find(headProThree) != headProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = headProThree;
			return headProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(headProTwoVector.at(index)->find(numberOfSyllables)->second.find(headProTwo) != headProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = headProTwo;
				return headProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = headProOne;
				return headProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case ADVERB:
		index = 1;
		if(headProThreeVector.at(index)->find(numberOfSyllables)->second.find(headProThree) != headProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = headProThree;
			return headProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(headProTwoVector.at(index)->find(numberOfSyllables)->second.find(headProTwo) != headProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = headProTwo;
				return headProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = headProOne;
				return headProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case NOUN:
		index = 2;
		if(headProThreeVector.at(index)->find(numberOfSyllables)->second.find(headProThree) != headProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = headProThree;
			return headProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(headProTwoVector.at(index)->find(numberOfSyllables)->second.find(headProTwo) != headProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = headProTwo;
				return headProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = headProOne;
				return headProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case VERB:
		index = 3;
		if(headProThreeVector.at(index)->find(numberOfSyllables)->second.find(headProThree) != headProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = headProThree;
			return headProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(headProTwoVector.at(index)->find(numberOfSyllables)->second.find(headProTwo) != headProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = headProTwo;
				return headProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = headProOne;
				return headProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	}
}

multimap<string, string>& RhymeBot::getTailProMap(PART_OF_SPEECH partOfSpeech, int numberOfSyllables, string word){


	PART_OF_SPEECH pos = partOfSpeech;
	int index;
	string headProOne = "" +  dictionaryMap.find(word)->second.at(0).getSourceString();
	string tailProOne = "" + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-1).getSourceString();
	string headProTwo = "";
	string tailProTwo = "";
	string headProThree = "";
	string tailProThree = "";
	if(dictionaryMap.find(word)->second.size() >= 2){
		headProTwo = "" + headProOne +  dictionaryMap.find(word)->second.at(1).getSourceString();
		tailProTwo = "" + tailProOne + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-2).getSourceString();
	}

	if(dictionaryMap.find(word)->second.size() >= 3){
		headProThree = "" + headProTwo +  dictionaryMap.find(word)->second.at(2).getSourceString();
		tailProThree = "" + tailProTwo + dictionaryMap.find(word)->second.at(dictionaryMap.find(word)->second.size()-3).getSourceString();
	}
	switch(pos){
	case ADJECTIVE:
		index = 0;
		if(tailProThreeVector.at(index)->find(numberOfSyllables)->second.find(tailProThree) != tailProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = tailProThree;
			return tailProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(tailProTwoVector.at(index)->find(numberOfSyllables)->second.find(tailProTwo) != tailProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = tailProTwo;
				return tailProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = tailProOne;
				return tailProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case ADVERB:
		index = 1;
		if(tailProThreeVector.at(index)->find(numberOfSyllables)->second.find(tailProThree) != tailProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = tailProThree;
			return tailProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(tailProTwoVector.at(index)->find(numberOfSyllables)->second.find(tailProTwo) != tailProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = tailProTwo;
				return tailProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = tailProOne;
				return tailProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case NOUN:
		index = 2;
		if(tailProThreeVector.at(index)->find(numberOfSyllables)->second.find(tailProThree) != tailProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = tailProThree;
			return tailProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(tailProTwoVector.at(index)->find(numberOfSyllables)->second.find(tailProTwo) != tailProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = tailProTwo;
				return tailProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = tailProOne;
				return tailProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	case VERB:
		index = 3;
		if(tailProThreeVector.at(index)->find(numberOfSyllables)->second.find(tailProThree) != tailProThreeVector.at(index)->find(numberOfSyllables)->second.end()){
			toQuery = tailProThree;
			return tailProThreeVector.at(index)->find(numberOfSyllables)->second;
		}
		else{
			if(tailProTwoVector.at(index)->find(numberOfSyllables)->second.find(tailProTwo) != tailProTwoVector.at(index)->find(numberOfSyllables)->second.end()){
				toQuery = tailProTwo;
				return tailProTwoVector.at(index)->find(numberOfSyllables)->second;
			}
			else{
				toQuery = tailProOne;
				return tailProVector.at(index)->find(numberOfSyllables)->second;
			}
		}
		//		return headProTwoVector.at(index)->find(numberOfSyllables)->second;
		break;
	}
}

void RhymeBot::printMap(PART_OF_SPEECH partOfSpeech, int numberOfSyllables){
	PART_OF_SPEECH pos = partOfSpeech;
	pair<multimap<int, string>::iterator, multimap<int, string>::iterator> range;
	switch(pos){
	case ADJECTIVE:
		range = partOfSpeechVector.at(0)->equal_range(numberOfSyllables);
		for(multimap<int, string>::iterator it = range.first; it != range.second; it++){
			cout << "ADJ" << endl;
			cout << it->first << ", " << it->second << endl;
		}
		break;
	case ADVERB:
		range = partOfSpeechVector.at(1)->equal_range(numberOfSyllables);
		for(multimap<int, string>::iterator it = range.first; it != range.second; it++){
			cout << "ADV" << endl;
			cout << it->first << ", " << it->second << endl;
		}
		break;
	case NOUN:
		range = partOfSpeechVector.at(2)->equal_range(numberOfSyllables);
		for(multimap<int, string>::iterator it = range.first; it != range.second; it++){
			cout << "NOUN" << endl;
			cout << it->first << ", " << it->second << endl;
		}
		break;
	case VERB:
		range = partOfSpeechVector.at(3)->equal_range(numberOfSyllables);
		for(multimap<int, string>::iterator it = range.first; it != range.second; it++){
			cout << "VERB" << endl;
			cout << it->first << ", " << it->second << endl;
		}
		break;
	}
}

string RhymeBot::theBest(int numberOfSyllables, PART_OF_SPEECH partOfSpeech, string rhymesWith, string alliterationWith){

	string headProOne = "" +  dictionaryMap.find(alliterationWith)->second.at(0).getSourceString();
	string tailProOne = "" + dictionaryMap.find(rhymesWith)->second.at(dictionaryMap.find(rhymesWith)->second.size()-1).getSourceString();
	string headAndTail = "" + headProOne + tailProOne;
	string perfectWord = "";
	int perfectSum = 0;
	int index;
	long long theKey;
	pair<multimap<long long, string>::const_iterator, multimap<long long, string>::const_iterator> range;
	PART_OF_SPEECH pos = partOfSpeech;

	switch(pos){
	case ADJECTIVE:
		index = 0;
		theKey = hasher(headAndTail);
		range = superHasher.at(index)->find(numberOfSyllables)->second.equal_range(theKey);
		for(multimap<long long, string>::const_iterator it = range.first; it != range.second; it++){
			if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
				if(it->second != rhymesWith && it->second != alliterationWith){
					perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
					perfectWord = it->second;
				}
			}
		}
		break;
	case ADVERB:
		index = 1;
		theKey = hasher(headAndTail);
		range = superHasher.at(index)->find(numberOfSyllables)->second.equal_range(theKey);
		for(multimap<long long, string>::const_iterator it = range.first; it != range.second; it++){
			if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
				if(it->second != rhymesWith && it->second != alliterationWith){
					perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
					perfectWord = it->second;
				}
			}
		}
		break;
	case NOUN:
		index = 2;
		theKey = hasher(headAndTail);
		range = superHasher.at(index)->find(numberOfSyllables)->second.equal_range(theKey);
		for(multimap<long long, string>::const_iterator it = range.first; it != range.second; it++){
			if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
				if(it->second != rhymesWith && it->second != alliterationWith){
					perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
					perfectWord = it->second;
				}
			}
		}
		break;
	case VERB:
		index = 3;
		theKey = hasher(headAndTail);
		range = superHasher.at(index)->find(numberOfSyllables)->second.equal_range(theKey);
		for(multimap<long long, string>::const_iterator it = range.first; it != range.second; it++){
			if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
				if(it->second != rhymesWith && it->second != alliterationWith){
					perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
					perfectWord = it->second;
				}
			}
		}
		break;
	}

	//		multimap<string, string> tempMap = getHeadProMap(partOfSpeech, numberOfSyllables, alliterationWith);
	//		cout << "USE " << toQuery << " to query HEAD for " << alliterationWith << endl;
	//		pair<multimap<string, string>::iterator, multimap<string, string>::iterator> range = tempMap.equal_range(toQuery);
	//		for(map<string, string>::iterator it = range.first; it != range.second; it++){
	//			if(rhymingSyllables(rhymesWith, it->second) > 0 && alliterationSounds(alliterationWith, it->second) > 0){
	//				if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
	//					if(it->second != rhymesWith && it->second != alliterationWith){
	//						perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
	//						perfectWord = it->second;
	//					}
	//				}
	//			}
	//		}
	//
	//		tempMap = getTailProMap(partOfSpeech, numberOfSyllables, rhymesWith);
	//		cout << "USE " << toQuery << " to query TAIL for " << rhymesWith << endl;
	//		range = tempMap.equal_range(toQuery);
	//		for(multimap<string, string>::iterator it = range.first; it != range.second; it++){
	//			if(rhymingSyllables(rhymesWith, it->second) > 0 && alliterationSounds(alliterationWith, it->second) > 0){
	//				if(rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second) > perfectSum){
	//					if(it->second != rhymesWith && it->second != alliterationWith){
	//						perfectSum = rhymingSyllables(rhymesWith, it->second) + alliterationSounds(alliterationWith, it->second);
	//						perfectWord = it->second;
	//					}
	//				}
	//			}
	//		}

	return perfectWord;
}

void RhymeBot::checkProMap(){
	pair<multimap<string, string>::iterator, multimap<string, string>::iterator> rangeHead = headProTwoVector.at(3)->find(2)->second.equal_range("KEY1");
	pair<multimap<string, string>::iterator, multimap<string, string>::iterator> rangeTail = tailProTwoVector.at(3)->find(2)->second.equal_range("IY0K");
	pair<multimap<long long, string>::iterator, multimap<long long, string>::iterator> rangeHasher = superHasher.at(2)->find(2)->second.equal_range(hasher("SER0"));

	for(multimap<long long, string>::iterator it = rangeHasher.first; it != rangeHasher.second; it++){
		cout << it->first << ", " << it->second << endl;
	}

//	for(multimap<string, string>::iterator it = rangeHead.first; it != rangeHead.second; it++){
//		cout << it->first << ", " << it->second << endl;
//	}
//
//	for(multimap<string, string>::iterator it = rangeTail.first; it != rangeTail.second; it++){
//		cout << it->first << ", " << it->second << endl;
//	}

	//	for(multimap<string, string>::iterator it = tailProThreeVector.at(3)->find(2)->second.begin(); it!= tailProThreeVector.at(3)->find(2)->second.end(); it++){
	//		cout << it->first << ", " << it->second << endl;
	//	}

	//	for(multimap<string, string>::iterator it = tailProThreeVector.at(3)->find(2)->second.begin(); it!= tailProThreeVector.at(3)->find(2)->second.end(); it++){
	//		cout << it->first << ", " << it->second << endl;
	//	}
}
