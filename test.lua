{
  config = {
    1 = {
      type = "STRING",
      value = "MULTILINE\nstring\"With quotes",
      name = "Edit me"
    },
    2 = {
      type = "NUMBER",
      value = 12,
      name = "Just a number"
    },
    3 = {
      kids = {
        1 = {
          type = "NUMBER",
          value = 11,
          name = "Num 1"
        },
        2 = {
          type = "NUMBER",
          value = 22,
          name = "Num 2"
        }
      },
      type = "BOOLEAN",
      value = true,
      name = "Boolean"
    }
  },
  name = "Preferences"
}