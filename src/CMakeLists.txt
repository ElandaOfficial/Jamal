target_sources(${JAMAL_PROJECT_TARGET}
    PRIVATE
        ### Main classes
        Main.cpp
        MainComponent.cpp
        
        ### Editor
        editor/CodeEditor.cpp
        
            ## Tokeniser
            editor/syntax/TokenIterator.cpp
            editor/syntax/JamlTokeniser.cpp
            editor/syntax/JamlTokeniserUtils.cpp
            
                # Proxies
                editor/syntax/proxies/ProxyCDataBlock.cpp
                editor/syntax/proxies/ProxyEntity.cpp
                editor/syntax/proxies/ProxyLiteral.cpp
                editor/syntax/proxies/ProxyOperator.cpp
                editor/syntax/proxies/ProxyTagOperator.cpp
                editor/syntax/proxies/ProxyText.cpp)
