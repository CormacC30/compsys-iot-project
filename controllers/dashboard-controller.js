export const dashboardController = {

    async index(request, response) {
        const viewData = {
            title: "Dashboard",
            stations: stations,
        };

        console.log("rendering dashboard");
        response.render("dashboard-view", viewData);
    },
};