# Dependencies

-   PostgreSQL v15.3
-   Redis v7.0.11
-   NodeJS v20.2.0
-   npm v9.6.7
-   Other dependencies are listed in [package.json](package.json)

# Startup

-   Install the dependencies listed above (using brew or apt)
-   Run `npm install` to install additional dependencies listed in [package.json](package.json)
-   Specify the required settings in [common/configs/config.json](common/configs/config.json)
    -   `/stage`: `testing` or `release`
    -   `/application` is the name of the application to specify when connecting to the databases
    -   `/postgreSQL` hold postgreSQL connection information
    -   `/port` specify the ports for the web server to run on
-   Run `npm run db:full` to create the required database
-   Run `npm run service` to start the service

# Documentation

-   Endpoints description is given as swagger documentation stored in [documentation folder](documentation)
-   Database setup scripts can be found in [databases/setup.sql](databases/setup.sql)
