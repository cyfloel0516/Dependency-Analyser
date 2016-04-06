devenv ".\Lexical Scanner.sln" /build DEBUG

xcopy /s/y Tokenizer_Test_Console\test_files Debug\test_files\*
xcopy /s/y Tokenizer\token_config.txt Debug\*
xcopy /s/y SemiExpression\ignore_config.txt Debug\*