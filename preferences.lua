

function alt_preferences_get()

    pref = {}

    -- function pref.save()
    --     print("Oh yeah")
    -- end

    pref.syntax = { config = {}, highlighting = {} }

    pref.config = {}

    pref.syntax.highlighting.palette = {
        background = '#FFFFFF',
        text = '#000000',
        character = '#808080',
        numeric = '#808030'
    }

    pref.syntax.highlighting.html = {
        root = {
            background = "background",
            foreground = "text",
            patterns = {
                {
                    pattern = "\\<\\?php",
                    action = "ENTER",
                    name = "php"
                }
            }
        },
    }

    return(pref)

end



function alt_preferences_set(pref)

    print("alt_preferences_set")
    return(true)

end

