export const dashboardController = {

    async index(request, response) {
        const viewData = {
            title: "Dashboard",
        };

        console.log("rendering dashboard");
        response.render("dashboard", viewData);
    },
};