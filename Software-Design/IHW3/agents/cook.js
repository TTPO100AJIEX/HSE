import { KitchenClient } from "utils/grpc/client.js";
import { CookServer } from "utils/grpc/server.js";
import sleep from "utils/sleep.js";

export default class Cook extends CookServer {
	#name;
	constructor(port, kitchenPort, name) {
		super(port);
		this.#name = name;
		const kitchen = new KitchenClient(kitchenPort);
		kitchen.registerCook({ port });
		kitchen.close();
	}

	async cook(data, callback) {
		// DO
		console.log(data.request);
		await sleep(7000);
		callback(null, { food: "test" });
	}
}

const cook = new Cook(process.argv[2], process.argv[3], process.argv.slice(4).join(" "));
