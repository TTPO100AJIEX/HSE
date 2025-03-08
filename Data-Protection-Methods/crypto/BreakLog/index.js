import BreakLogDumb from "./BreakLogDumb.js";
import BreakLogSmart from "./BreakLogSmart.js";
import { clear_cache } from "./utils/pow.js";

for (let i = 0; i < 1e9; i++) { i -= 0.5; } // warmup

var tests = [ [ 41, 6, 30 ], [ 999999937, 543791, 94517380 ]];

for (let test of tests)
{
    clear_cache();

    console.time('Dumb solution');
    let res1 = BreakLogDumb(...test);
    console.timeEnd('Dumb solution');
    
    console.time('Smart solution');
    let res2 = BreakLogSmart(...test);
    console.timeEnd('Smart solution');
    
    console.log(res1, res2);
}