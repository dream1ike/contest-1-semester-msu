void generate_binary_sets(int num, int order, int **coeffs){
	int set[num];

	for(int idx=0; idx<num; idx++){set[idx]=0;}

	do
	{
		print_set(set, num);
		int position=num-1;

		while(set[position] == 1 && position > 0) { position--; }

		set[position] = 1;

		for(int i = position+1; i < num; i++){
			set[i] = 0;
			print_set(set, num);
		}
	}
	while( set[0] != 1 );

	print_set(set, num);
}

void print_set(int* set, int num) {
	for(int i = 0; i < num; i++) {
		printf("%d ", set[i]);
	}
	printf("\n");
}
