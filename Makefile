make: lib/Main_Body.lib lib/DFA.lib lib/Result_Data_Structure.lib 
	g++ lib/Main_Body.lib lib/DFA.lib lib/Result_Data_Structure.lib -o bin/LexicalAnalyzer.exe

lib/Main_Body.lib: src/Main_Body.cpp
	g++ -c src/Main_Body.cpp -o lib/Main_Body.lib

lib/DFA.lib: src/DFA.cpp
	g++ -c src/DFA.cpp -o lib/DFA.lib

lib/Result_Data_Structure.lib: src/Result_Data_Structure.cpp
	g++ -c src/Result_Data_Structure.cpp  -o lib/Result_Data_Structure.lib

run: bin/LexicalAnalyzer.exe
	./bin/LexicalAnalyzer.exe
