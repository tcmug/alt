{
  config = {
    1 = {
      value = "MULTILINE\nstring\"With quotes",
      type = "STRING",
      name = "Edit me"
    },
    2 = {
      value = 12,
      type = "NUMBER",
      name = "Just a number"
    },
    3 = {
      kids = {
        1 = {
          value = 11,
          type = "NUMBER",
          name = "Num 1"
        },
        2 = {
          value = 22,
          type = "NUMBER",
          name = "Num 2"
        }
      },
      value = true,
      type = "BOOLEAN",
      name = "Boolean"
    }
  },
  name = "Preferences"
}