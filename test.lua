{
  name = "Preferences",
  config = {
    1 = {
      type = "STRING",
      name = "Edit me",
      value = "MULTILINE\nstring\"With quotes"
    },
    2 = {
      type = "NUMBER",
      name = "Just a number",
      value = 12
    },
    3 = {
      kids = {
        1 = {
          type = "NUMBER",
          name = "Num 1",
          value = 11
        },
        2 = {
          type = "NUMBER",
          name = "Num 2",
          value = 22
        }
      },
      type = "BOOLEAN",
      name = "Boolean",
      value = true
    }
  }
}