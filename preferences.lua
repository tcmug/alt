

alt = {
    cfg = {

        str = function (name, default, kids)
            return ( { name=name, value=default, type="STRING", kids=kids} )
        end,

        bool = function (name, default, kids)
            return ( { name=name, value=default, type="BOOLEAN", kids=kids} )
        end,

        num = function  (name, default, kids)
            return ( { name=name, value=default, type="NUMBER", kids=kids} )
        end,

        save = function (file, cfg)
            local file = io.open(file, "w")
            alt.cfg._write(file, cfg, 1);
            file:close()
        end,

        _write = function (file, item, depth)
            file:write("{\n")
            local prefix = string.rep("  ", depth)
            for key, value in pairs(item) do
                local pair = prefix .. key .. " = "
                if (type(value) == "table") then
                    -- Handle tables - usually child elements.
                    file:write(pair)
                    alt.cfg._write(file, value, depth + 1)
                else
                    -- Everything else:
                    if (type(value) == "string") then
                        pair = pair .. "\"" .. value:gsub("\"", "\\\""):gsub("\n", "\\n"):gsub("\t", "\\t"):gsub("\r", "\\r") .. "\""
                    elseif (type(value) == "number") then
                        pair = pair .. value
                    elseif (type(value) == "boolean") then
                        if (value) then
                            pair = pair  .. "true"
                        else
                            pair = pair .. "false"
                        end
                    end
                    file:write(pair)
                end
                prefix = ",\n" .. string.rep("  ", depth)
            end
            file:write("\n" .. string.rep("  ", depth - 1) .. "}")
        end
    }
}

function alt_preferences_get()

    local cfg = {
        name = "Preferences",
        config = {
            alt.cfg.str("Edit me", "MULTILINE\nstring\"With quotes"),
            alt.cfg.num("Just a number", 12),
            alt.cfg.bool("Boolean", true, {
                alt.cfg.num("Num 1", 11),
                alt.cfg.num("Num 2", 22)
            })
        }
    }

    alt.cfg.save("test.lua", cfg)

    return (cfg)

end



function alt_preferences_set(pref)

    print("alt_preferences_set")
    return(true)

end


--local foo = Foo("a", "b")
--foo:foo()

