

function pref_string(name, default, kids)
    return ( { name=name, value=default, type="STRING", kids=kids} )
end

function pref_boolean(name, default, kids)
    return ( { name=name, value=default, type="BOOLEAN", kids=kids} )
end

function pref_number(name, default, kids)
    return ( { name=name, value=default, type="NUMBER", kids=kids} )
end

function alt_preferences_get()

    pref = {}

    -- function pref.save()
    --     print("Oh yeah")
    -- end

    -- pref.syntax = { config = {}, highlighting = {} }

    pref.name = "Preferences"

    pref.config = {
        pref_string("Edit me", "name"),
        pref_number("Just a number", 12),
        pref_boolean("Boolean", true, {
            pref_number("Num 1", 11),
            pref_number("Num 2", 22)
        }),
    }

    -- pref.syntax.highlighting.palette = {
    --     background = '#FFFFFF',
    --     text = '#000000',
    --     character = '#808080',
    --     numeric = '#808030'
    -- }

    -- pref.syntax.highlighting.html = {
    --     root = {
    --         background = "background",
    --         foreground = "text",
    --         patterns = {
    --             {
    --                 pattern = "\\<\\?php",
    --                 action = "ENTER",
    --                 name = "php"
    --             }
    --         }
    --     },
    -- }

    return(pref)
end



function alt_preferences_set(pref)

    print("alt_preferences_set")
    return(true)

end

