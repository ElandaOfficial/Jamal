target_sources(${JAMAL_PROJECT_TARGET}
    PRIVATE
        ### Main classes
        Main.cpp
        MainComponent.cpp
        
        ### Editor
        editor/CodeEditor.cpp
        editor/TextView.cpp
            ## Render
            editor/render/TextViewLayout.cpp
            
            ## Syntax
                # TextMate
                editor/syntax/textmate/TextMateCache.cpp
                editor/syntax/textmate/TextMateParser.cpp)
