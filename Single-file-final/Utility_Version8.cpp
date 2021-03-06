#include<stdio.h>
#include<string.h>

void search(char [], char[]); 
/* 
Search if a given string is present in the given input Hex file
Input arguments : Filename and the string to be searched
Output : 
		whether string is present in file or not.
		if present, then at what byte it is present.
*/
void compare_c(char [], char []); 
/*
 compares the contents of two hex files as input and displays if the two files are equivalent
Input  arguments: Two Intel Hex format filenames
Output : 
		if content is equal
			display The files are identical
		else if contents are not equal
			display byte number at which the characters are different
 */
void verify(char []); 
/*
verifies whether the Input file is in Intel Hex file format 
Input  arguments: One Intel hex filename
Output :
		Display whether it is in Intel Hex file format or not.
			(Display if there is a checksum error)
*/
void convert(char [],char []); 
/*
Converts the records of an Intel hex file to a different set of records with a max byte count restriction and displays them
Input arguments: Intel Hex filename and max byte count
Output : Records with a set max byte count
*/
void crc(char []);
/*
Displays the 8-bit CRC of the contents(Data) of the file 
Input arguments: Filename with Intel Hex records
Output : Display CRC is binary and Hexadecimal format
*/
void convert_to_binary(char []);
/*
Convert the contents of an input Intel Hex file into a binary data
Input arguments: An Intel Hex filename
Output : Display the contents of the file converted into binary digits
*/
void convert_to_hex(char [], int);
/*
Converts the contents of a binary file into Intel Hex records with incremental addresses with max byte count given(only 16/32 bytes)
Input arguments: A binary filename and size(16/32 bytes)
Output : Display the records in Intel Hex format with given max byte count
*/
int checksum(int [], int size, int cs);
/*
Calculates the checksum of the Intel Hex record and compare it with the checksum already present in the record to check if there
is a check sum error.
Input arguments: Integer array of (byte count, address, record type and data) , size of the record( byte count + 4) , already present checksum in the record
 *4+byte count is sent for check sum calculation as byte count, address(2) and record type is also included in the calculation of the check sum*
Output : 
			if calculated checksum is equal to checksum in the record
				return 0
			else
				display 'Checksum error'
					return 1;					
*/
int ret_checksum(int [],int);
/*
Calculates the checksum of the Intel Hex record and return it.
Input arguments: Integer array of (byte count, address, record type and data) , size of the record( byte count + 4)  
*4+byte count is sent for check sum calculation as byte count, address(2) and record type is also included in the calculation of the check sum*
Output :
		Returns the checksum of the record	
*/
char get_c(char []); 
/*
returns character at position 1 in the character array
*/
int hex(char); 
/*
returns the decimal as an integer data type given a hexadecimal character
*/

/*
ASSUMPTIONS :
1. Max number of records per file is 100
*/
int comp_data[25500], bin_data[204000]; // Max size of a record is 255 bytes  and max number of records per file is assumed to be 100.
int flag =0;

struct dat // used in the function compare_c()
{
 	   int size;
 	   int address;
 	   int data[1000];
	   		   
}one[100], two[100]; // Assumtion : Max 100 records in each file

int main(int argc, char *argv[]) // argc -> argument count and argv -> argument values
{
 	if(argc == 1)
 	{
	 		printf("\nFormat : %s [-c file1 file2 ][-y file1][-r file1][-b file1][-h file1 byte_count][-v file1 max_byte_count][-s filename string]", argv[0]);
			printf("\n\nOptions : ");
			printf("\n\n[-c] \t - \t Compare 2 Intel Hex Binary files based on contents(data) present in the file. \n");
			printf("\n\n[-y] \t - \t Calculate the Cyclic Redundancy Check of one Intel Hex Binary file. \n");
			printf("\n\n[-r] \t - \t Verify whether a file is in Intel Hex Binary File format.  \n");
			printf("\n\n[-b] \t - \t Convert the given Intel Hex file to binary format  \n");
			printf("\n\n[-h] \t - \t Convert the given binary file to Intel Hex file format(byte count given)  \n");
			printf("\n\n[-v] \t - \t Convert the given Intel Hex file to another Intel Hex file with given max byte size limit  \n");
			printf("\n\n[-s] \t - \t Check if a given string is present in the contents of the Intel Hex file  \n");
		
		return 0;  
	}
 	else
	{
		switch(get_c(argv[1]))
	    {
		  					  case 'c' :
							  	   	   	if(argc == 4) // Number of arguments must be 4 [name_of_exe -c file1 file2] (4 arguments in total)
							  	   	   	{
													compare_c(argv[2],argv[3]);
									   	 }
										else
										{
										 	printf(" \n [-c file1 file2] \n");
									 	} 		   	 
										 break;
							case 'y' :
								 	  	if(argc == 3)
									    {
											 printf("\nThe Cyclic redundancy Check of the above Intel Hex File is : ");
											 crc(argv[2]);
								    	}
										else
										{	
											printf("\nUse the correct format [-y file1] \n");
										}				  			  		 
										break;
							case 'r' :
								 	    if(argc == 3)
									    {
									   		verify(argv[2]); 
								        }
										else
										{
											printf("\nUse the correct format [-r file1] \n");
										}
									break;		 						
							case 'b' :
										if(argc == 3)
										{
											convert_to_binary(argv[2]);
										}
										else
										{
											printf("\nUse the correct format [-b file1] \n");
										}
										break;
							case 'h' :
										if(argc == 4)
										{
											if(strcmp(argv[3],"16")==0)
											convert_to_hex(argv[2],16);
											else if(strcmp(argv[3],"32")==0)
											convert_to_hex(argv[2],32);	
											else
											{
												printf("Only 16 or 32 byte format allowed");
											}
										}
										else
										{
											printf("\nUse the correct format [-h file1 byte_count] \n");
										}
										break;
							case 'v' :
										if(argc==4)
										{
											convert(argv[2],argv[3]);
										}
										else
											printf("\nUse the correct format [-v file1 max_byte_count] \n");
										break;
							case 's' : 
										if(argc==4)
										{
											search(argv[2],argv[3]);
										}
										else
											printf("\nUse the correct format [-s filename string]");
										break;
							default  : 
										printf("\nInsufficient/Incorrect  Arguments \n");	
		}
		 
	}
 	return 0;
}

char get_c(char x[]) // removes '-' from the string which represents the options 
{
 	return x[1];
}

int hex(char c) // returns the decimal values(as an integer) from the input hexadecimal character
{
 	int res =0 ;
 	if(c >= '0' && c <='9')
 	res = c - '0';
 	else if(c >= 'A' && c <= 'F')
 	res = c - 'A' + 10;

 	return res;
}

int checksum(int dat[], int size, int cs) // calculate the check_sum value of the dat[] array and compare with cs(existing checksum)
{
	int i, p1, p2, check_sum;
 	int sum =0;
 	
 	for(i=0;i<size;++i)
 	{
	 	   sum += dat[i]; 
    }
	
	p1= sum%16;  
	sum = sum/16;
	p2 = sum%16;
	check_sum = 255 - (p2*16 + p1 ) + 1; // two's complement for hexadecimal values
	p1 = check_sum %16;
	check_sum = check_sum/16;
	p2 = check_sum %16;
	check_sum = p2 * 16 + p1;
	 
		if(check_sum == cs) // if the calculated checksum and the existing check sum are equal , return 0
			return 0; 				   
 	printf("\nThe check sum is %i while in the file it is %i \n Record is corrupt \n", check_sum, cs );
 	return 1; // if the calculated checksum and the existing check sum are not equal, return 1
}

int ret_checksum(int dat[], int size) // returns the check sum of the dat[] array 
{
	int i, p1, p2,check_sum;
	int sum =0;
 	
 	for(i=0;i<size;++i)
 	{
	 	   sum += dat[i];
    }
	
	p1= sum%16;
	sum = sum/16;
	p2 = sum%16;
	check_sum = 255 - (p2*16 + p1 ) + 1;
	p1 = check_sum %16;
	check_sum = check_sum/16;
	p2 = check_sum %16;
	check_sum = p2 * 16 + p1;
	return check_sum;
}	

void compare_c(char file1[], char file2[]) // compare only the contents(data) of the records present in the file.
{
 	 FILE *nf = fopen(file1, "r");
 	 char c;
	 int comp_data[1000], comp_data2[1000],check[267];
	 int i, j, k, c1, c2, c3, c4, add, rt, cs, bc, lf, j1=0,l=0, m=0, address =0,j2=0;
	 int tot, temp, ad1, ad2, ad3, ad4;
	 while(1)// Store only the contents(data) of file 1 in comp_data[] array
 	 {
					c = fgetc(nf);
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));
				 	c1 = hex(fgetc(nf));
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					for(j=0;j<bc;++j)
					{
					 	comp_data[j1++] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));		// Store the contents of the file1 in comp_data1 array and the contents of the file2 in the comp_data2 array 
				    }
					
				 	cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
				 	lf = fgetc(nf); 
					  
  		            if(lf==0xFFFFFFFF) // Represents the end of file
  				    {
	   				   break;
			        }
	 }
	 fclose(nf);
	 FILE *nf1 = fopen(file2 ,"r");
  	 while(1) // Store only the contents(data) of file 2 in comp_data2[] array
 	 {
					c = fgetc(nf1);
					bc = hex(fgetc(nf1)) * 16 + hex(fgetc(nf1));
				 	c1 = hex(fgetc(nf1));
				 	c2 = hex(fgetc(nf1));
				  	c3 = hex(fgetc(nf1));
				   	c4 = hex(fgetc(nf1));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf1))*16 + hex(fgetc(nf1));
					for(j=0;j<bc;++j)
					{
					 	comp_data2[j2++] = hex(fgetc(nf1)) * 16 + hex(fgetc(nf1));	
				    }				
				 	cs = hex(fgetc(nf1))*16 + hex(fgetc(nf1));
				 	lf = fgetc(nf1);  
  		            if(lf==0xFFFFFFFF) // Represents the end of file 
  				    {
	   				   break;
			        }
	 }
	  int flag = 0;
	  if(j1!=j2)
	 {
	  			 printf("The files are not identical\nThe contents are of different size");
	  			 flag = 1;
     }
	  else
	  {
	   	  for(i=0;i<j1;++i)
	   	  {
		   				   if(comp_data[i]!=comp_data2[i])
		   				   {
						   				printf("The files are not identical\nAs data at byte : %iS is is different",i);				   
				   		   				flag=1;
				   		   				break;
					  		}
   		  }
	  }
	  if(flag==0)
	  {
	   			 printf("The files are identical");
	   			 }  	 
	   
}
  

void verify(char file[]) // verifies whether the file is in the Intel Hex file format
{ 	
 	int j;
	int data[256];
	int check[267]; // (max data size) + 11(header length + check sum)
	char c;
	int c1, c2, c3, c4, i;
	int bc, rt, cs, lf, cr ; // byte count and record type and check sum, line feed and carriage return
	int add, rec_num=0;
	int stud_num =1;
	
	FILE *nf = fopen(file, "r");
    
	   while(flag==0)
       {
			   		c = fgetc(nf); // Hex records start with ":" character 
			   		
				    if(c!=':')
			   		{
					 	flag = 1;
						break;	  
			 		}
 		
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));  // Record data size (byte count) is denoted by 2 Hex digits
					check[0] = bc; 	// Used for checksum calculation
					if(flag == 1)
		 			break;	
					
				 	c1 = hex(fgetc(nf)); // Record address is denoted by 4 Hex digits
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					check[1] = c1*16 + c2; // Used for check Sum
					check[2] = c3*16 + c4; // Used for check Sum
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4; // Calculate the address values from 4 Hex digits ( Hex to decimal system)		
					if(flag == 1)
					break;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					check[3] = rt; // Used for checksum calculation
					if(rt>=6||rt<0)
					flag=1;
	
					if(flag == 1)
					break;
					
					for(j=0;j<bc;++j)
					{
					 		data[j] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));	
							   check[4+j] = data[j];  // Used for checksum calculation
				    }
					
					cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
					
					if(flag!=1)
					{
							 int size = 4 + bc;
							 int cs_verify = checksum(check,size,cs);  // checksum() returns 0 if the calculated checksum is equal to the checksum of the record
	
							if(cs_verify !=0)
							{
				 					  printf("\nCheck sum error, the record is corrupt \n");
									  flag=1;
				 					  break;
				 			}
				
								lf = fgetc(nf);
					  
	     	  			  	  if(lf==0xFFFFFFFF)
			  				  {
								  break;
							  }
					  			 rec_num++;
					 } 
					else
    			    break;
			int k;
		   for(k=0;k<256;++k)
		   	 check[k]=0;
					  
		  if(rt==1)
		  {
		 	  break;
		  }		 
   	} 
   		
		if(flag == 1)
		{
		 	printf("The current record : %i is not an Intel Hex Record ", rec_num + 1);	 		
	    }
		else
		{
			printf("All the records were in Intel Hex format \n"); 				   	
		}
	
 	 
}

void crc(char file[]) // Display the 8-bit CRC ( Cyclic Redundancy Check) of the contents in the file in binary and hexadecimal format 
{
 	 FILE *nf = fopen(file,"r");
	 char c;
	 int i, j, k;
 	 int c1, c2, c3, c4, add, rt, cs, bc, lf;
	 int j1=0;
 	 while(1)
 	 {
					c = fgetc(nf);
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));
				 	c1 = hex(fgetc(nf));
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					for(j=0;j<bc;++j)
					{
					 		comp_data[j1++] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));		 
				    }
					
				cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
				 lf = fgetc(nf);
					  
					  			  	  if(lf==0xFFFFFFFF)
					  				  {
					   				   break;
				    				   }
			  		   		 
	 }
	 
 	 int ans[1000][8];
 	 int x1 =0;
 	 int n;
		for( k=0;k<j1;++k)             // Convert each element of the array into an eight bit binary sequence 
		{
			for( i=0;i<8;++i)
			{
				if(comp_data[k]%2==0)
				{
					ans[k][7-i]= 0;   
				}	 
				else
					ans[k][7-i] = 1;
		
		
			comp_data[k] = comp_data[k]/2;	    
			}
			for(n=0;n<8;++n)
			{
	  				 bin_data[x1++] = ans[k][n]; // Store the 8 bits generated 
			}
		}

		for(i=0;i<8;++i)
		{
			 bin_data[x1+i] = 0;
		}
		
	 int poly[] = {1,0,0,1,1,0,1,0,1}; // Initialize the CRC Polynomial (9 bits required for CRC 8)
	 int crc[8];
	 for(i=0;i<8;++i)
	 crc[i] = bin_data[i];
 
	 for(i=0;i<x1;++i) // Calculating the CRC using XOR division
	 {	  				
	  	  if(crc[0]==1)
		  {
		   			 for(j=0;j<8;++j)
		   			 {
					  				 if(j+1 != 8)
					  				 crc[j] = crc[j +1]^poly[j+1];
					  				 else
					  				 crc[j] = bin_data[i+j+1]^poly[j+1];
					  }
					  				 
		  }
		  else
		  {
		  	 for(j=0;j<8;++j)
		  	 {
			  				 if(j+1 !=8)
			  				 crc[j] = crc[j+1];
			  				 else
			  				 crc[j] = bin_data[i+j+1];
			  }
	      } 
							
    }
	 printf("\nCRC in BIN : ");
	 for(i=0;i<8;++i)
	 printf("%i ",crc[i]); 
 
	 c1 = crc[0]*2*2*2 + crc[1]*2*2 + crc[2]*2 + crc[3];
	 c2 = crc[4]*2*2*2 + crc[5]*2*2 + crc[6]*2 + crc[7];
	printf("\nCRC in HEX : %01X%01X",c1,c2);
	
}
void convert_to_binary(char filename[]) // converts the contents of the given Intel hex file to  binary data
{
	 FILE *nf = fopen(filename ,"r");
	 char c;
	 int c1, c2, c3, c4, add, rt, cs, bc, lf ,ans[1000][8], n, j, i, k, x1=0, j1=0;
 	 while(1)
 	 {
					c = fgetc(nf); 		
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));
				 	c1 = hex(fgetc(nf));
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					for(j=0;j<bc;++j)
					{
					 		comp_data[j1++] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));		 // only the data of each record is stored in comp_data[] array
				    }

					cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
					lf = fgetc(nf);
					  
					  			  	  if(lf==0xFFFFFFFF)
					  				  {
					   				   break;
				    				   }
			  		   		 
	 }
	 
	for(k=0;k<j1;++k)  // Data stored in  comp_data[] array is then converted to the binary format
	{
		for(i=0;i<8;++i) 
		{
			if(comp_data[k]%2==0)
			{
				ans[k][7-i]= 0; 
			}	 
			else
				ans[k][7-i] = 1;
	
		comp_data[k] = comp_data[k]/2;	    
		}
		for(n=0;n<8;++n)  // each element of the array is converted into 8-bit binary and printed out
		{
	  				 bin_data[x1++] = ans[k][n];
					 printf("%i",bin_data[x1-1]);
		}
	}


}

void convert_to_hex(char filename[], int bc) // Converts the contents of a binary file into Intel Hex records with incremental addresses with max byte count given(only 16/32 bytes) 
{
	FILE *nf = fopen(filename,"r");
	int a[8]; 
	int data[32], check[267]; // check array used for calculating check sum (max data size = 256 bytes and format size = 11 bytes)
	int c, ad1,ad2,ad3,ad4, temp;
	int i,size=0,address=0,j=0;
	while((c=fgetc(nf))!=0xFFFFFFFF)
	{
		a[0]=(int)(c - '0');   // get 8 binary digits at a time
		for(i=0;i<7;++i)
		{
			c = fgetc(nf); 	
			a[i+1] = (c - '0'); // convert character 0 or 1 to integer 0 or 1
		}
		data[size++] = a[0]*128 + a[1]*64 + a[2]*32 + a[3]*16 + a[4]*8 + a[5]*4 + a[6]*2 + a[7]; // convert 8-bit binary to decimal 
		if(size==bc)
		{
			printf(":%02X%04X%02X",size,address,0); 
			check[0]= size; // used for check sum
			check[1]= address; // used for check sum
			temp = address; // converting the address to 4 Digit Hex values and divide it into 2 bytes for calculation required for checksum
			ad1  = temp%16;
			temp = temp/16;
			ad2  = temp%16;
			temp = temp/16;
			ad3  = temp%16;
			temp = temp/16;
			ad4  = temp%16;
			check[1] = ad4*16 + ad3; // used for check sum
			check[2] = ad2*16 + ad1; // used for check sum
			check[3]= 0; // Record Type is set to 0 as all the records contain on data
			address++;	// Incremental addresses are used
			for(j=0;j<size;++j) // prints 16/32 bytes at a time
			{
				printf("%02X",data[j]);
				check[4+j] = data[j];
			}
			printf("%02X",ret_checksum(check,4+size)); // 4+size is sent for check sum calculation as byte count, address(2) and record type is also included in the calculation of the check sum
			size =0;
			printf("\n");
			
		}
			
	}
	if(size>0) // the extra bytes remaining 
	{
			printf(":%02X%04X%02X",size,address,0);
			check[0]= size; 
			temp = address; // converting the address to 4 Digit Hex values and divide it into 2 bytes for calculation required for checksum
			ad1  = temp%16;     
			temp = temp/16;
			ad2  = temp%16;
			temp = temp/16;
			ad3  = temp%16;
			temp = temp/16;
			ad4  = temp%16;
			check[1] = ad4*16 + ad3; // used for check sum
 			check[2] = ad2*16 + ad1; // used for check sum
			check[3]= 0; // Record Type is set to 0 as all the records contain on data
			address++;	// Incremental addresses are used
			
			for(j=0;j<size;++j)
			{
				printf("%02X",data[j]);
				check[4+j] = data[j]; // used for check sum
			}
			
			printf("%02X",ret_checksum(check,4+size)); // 4+size is sent for check sum calculation as byte count, address(2) and record type is also included in the calculation of the check sum
			
	}
}

void convert(char file[], char s[]) // convert the Intel Hex file to another Intel hex file with max byte size given by s 
{
 	 int size;
	 FILE *nf = fopen(file,"r");
	 char c;
	 int comp_data[1000], check[267];
	 int i, j, k, c1, c2, c3, c4, add, rt, cs, bc, lf, j1=0,l=0, m=0, address =0;;
	 int tot, temp, ad1, ad2, ad3, ad4;

	if(strlen(s)==1) // converting string s to and integer size to use for calculation
 	 size = s[0] - '0';
 	else if(strlen(s)==2)
	 size = (s[0] - '0')*10 + (s[1]-'0');
    else if(strlen(s)==3)
	 size = (s[0] - '0')*100 + (s[1] - '0')*10 + (s[2]-'0');
    else 
	 size = 256; 
	  
	if(size>256)  // Max byte count of an Intel Hex File is 256 bytes
	 size = 256;
	 
 	while(1)
 	{
					c = fgetc(nf);
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));
				 	c1 = hex(fgetc(nf));
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					for(j=0;j<bc;++j)
					{
					 	comp_data[j1++] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));	// To store only the contents (data) of the file in comp_data[] array
				    }
					
				 	cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
				 	lf = fgetc(nf); 
					  
  		            if(lf==0xFFFFFFFF)
  				    {
	   				   break;
			        }
	}
	   tot = j1; // The total number of bytes of data(ONLY CONTENT) present in the input file
	   while(tot>=size)
	   {
            printf(":%02X%04X%02X",size,address,0);
	   	    check[0]= size;
			temp = address; // converting the address to 4 Digit Hex values and divide it into 2 bytes for calculation required for checksum
			ad1  = temp%16;
			temp = temp/16;
			ad2  = temp%16;
			temp = temp/16;
			ad3  = temp%16;
			temp = temp/16;
			ad4  = temp%16;
			check[1] = ad4*16 + ad3; // used for check sum
			check[2] = ad2*16 + ad1; // used for check sum
			check[3]= 0; // Record Type is set to 0 as all the records contain on data
			address++;	// Incremental addresses are used	     
   			for(l=0;l<size;++l)
 		    {
			   	printf("%02X",comp_data[l + m]);
			   	check[4+l] = comp_data[l + m];
		    }
		    m+=size;
		    printf("%02X\n",ret_checksum(check,4+size));
		    tot-=size;
	   }
	   if(tot>0)  // The data which is less the max size required
	   {
 		    printf(":%02X%04X%02X",tot,address,0);
	   	    check[0]= tot;
			temp = address; // converting the address to 4 Digit Hex values and divide it into 2 bytes for calculation required for checksum
			ad1  = temp%16;
			temp = temp/16;
			ad2  = temp%16;
			temp = temp/16;
			ad3  = temp%16;
			temp = temp/16;
			ad4  = temp%16;
			check[1] = ad4*16 + ad3; // used for check sum
			check[2] = ad2*16 + ad1; // used for check sum
			check[3] = 0; // Record Type is set to 0 as all the records contain on data
			address++;	// Incremental addresses are used	     
   			for(l=0;l<tot;++l)
 		    {
			   	printf("%02X",comp_data[l + m]);
			   	check[4+l] = comp_data[l + m]; 
		    }
		    m+=size;
		    printf("%02X",ret_checksum(check,4+tot));
		}	    
}
void search(char filename[], char s[]) // search and check if the given string s is present in the Intel hex file 
{
	int i=0, j=0, flag=0;
	int c, bc, c1, c2, c3, c4, add, rt, comp_data[1000], cs, lf, j1=0;
	FILE *nf = fopen(filename,"r");
	while(1)
 	{
					c = fgetc(nf);
					bc = hex(fgetc(nf)) * 16 + hex(fgetc(nf));
				 	c1 = hex(fgetc(nf));
				 	c2 = hex(fgetc(nf));
				  	c3 = hex(fgetc(nf));
				   	c4 = hex(fgetc(nf));
					add = c1 * 16 * 16 * 16 + c2 * 16 * 16  + c3 * 16 + c4;
					rt = hex(fgetc(nf))*16 + hex(fgetc(nf));
					for(j=0;j<bc;++j)
					{
					 	comp_data[j1++] = hex(fgetc(nf)) * 16 + hex(fgetc(nf));		// Ignore Everything but the contents of the file as we only need to search the data in the file
				    }
				 	cs = hex(fgetc(nf))*16 + hex(fgetc(nf));
				 	lf = fgetc(nf);   
  		            if(lf==0xFFFFFFFF)
  				    {
	   				   break;
			        }
	}
	i=0;
	j=0;
	flag=0;
	for(i=0;i<j1;++i) // Simple brute force string searching 
	{
		if(comp_data[i]==(int)s[0])
		for(j=0;j<strlen(s);++j)
		{
		
			if((int)s[j]!=comp_data[i+j])
			{
				break;
			}
			if(j== strlen(s) -1 )
			{
				flag =1;
				break;
			}	
		}
		if(flag==1)
			break;
	}
	if(flag==0)
	printf("\nThe string doesnt exist in the contents of the file");
	else
	printf("\nThe string exists in the file at byte %i", i);
}
