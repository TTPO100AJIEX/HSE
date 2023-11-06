import fastify_plugin from "fastify-plugin";

import fastify_formbody from "@fastify/formbody";

function register(app, options, done) {
	/*----------------------------------FORMBODY----------------------------------*/
	app.register(fastify_formbody, { bodyLimit: 1048576 });

	done();
}

export default fastify_plugin(register, { name: "request", encapsulate: false });
