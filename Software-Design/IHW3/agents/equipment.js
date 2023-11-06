import { KitchenClient } from "utils/grpc/client.js";
import { EquipmentServer } from "utils/grpc/server.js";

export default class Equipment extends EquipmentServer {
	#type;
	#name;
	constructor(port, kitchenPort, type, name) {
		super(port);
		this.#type = type;
		this.#name = name;
		const kitchen = new KitchenClient(kitchenPort);
		kitchen.registerEquipment({ port, type });
		kitchen.close();
	}

	nothing(data, callback) {
		// DO
		console.log("!");
		callback(null, {});
	}
}

const equipment = new Equipment(process.argv[2], process.argv[3], process.argv[4], process.argv.slice(5).join(" "));
