function inicializar() {

	var curve = [
					[10000, 265],
					[8000,  268],
					[6000,  274],
					[4000,  281],
					[3000,  288],
					[2000,  293],
					[1500,  298],
					[1000,  303],
					[800,   307],
					[600,   314],
					[400,   324]
				];

	var i = 0;
	var j = 0;

	for( i = 0; i < curve.length - 1; i++ ) {

		var ppm0 = curve[i] [0];
		var ppm = curve[i+1][0];
		var mv0 = curve[i+1][1];
		var mv = curve [i]  [1];

		var diffppm = ppm0 - ppm;
		var diffmv = mv0 - mv;

		var step = (diffppm / diffmv);
		
		for( j = 0; j < diffmv; j++ ) {
			console.log( "milivolts = " + mv + " PPM = " + ppm0 );
			ppm0 -= step;
			mv ++;
		}

	}

}
