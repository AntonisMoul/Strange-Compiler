program first
 {

	declare a,b,m,n,o,p,c,k,l,d,v,h,y enddeclare
	
	
	
	procedure p1(inout c2,inout c3){

		procedure p2(in v3){
			print(v3);
			forcase{
				(when([v3 = 19]){
			
					print(v3);
					v3 := v3 + 1
				})
			
				(when([v3 = 18]){
			
					print(v3);
					v3 := v3 + 1
				})
			
				(when([v3 = 17]){
			
					print(v3);
					v3 := v3 + 1
				})
		
		
		
			}
		
		}

		forcase{
			(when([c2 = 15]){
			
				print(c2);
				c2 := c2 + 1
			})
			
			(when([c2 = 17]){
			
				print(c2);
				c2 := c2 + 1
			})
			
			(when([c2 = 16]){
			
				print(c2);
				c2 := c2 + 1
			})
		
		
		
		};
		

		
		call p2(in c2);
		print(c2)
	}

	
	
	a := 15;
	b := 20;
	o := 35;
	call p1(inout a, inout b);
	print(a);
	incase{
			(when([a = 21]){
			
				print(a);
				a := a + 1
			})
			
			(when([a = 20]){
			
				print(a);
				a := a + 1
			})
		
			
			(when([a = 22]){
			
				print(a);
				a := a + 1
			})
		
		
		
		}
	

}
