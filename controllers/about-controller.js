export const aboutController = {
    index(request, response) {
      const viewData = {
        title: "About Mould-O-Matic",
      };
      console.log("about rendering");
      response.render("about", viewData);
    },
  };
  