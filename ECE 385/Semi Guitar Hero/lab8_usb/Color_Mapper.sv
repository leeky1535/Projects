//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//                                                                       --
//    Fall 2014 Distribution                                             --
//                                                                       --
//    For use with ECE 385 Lab 7                                         --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------


module  color_mapper ( input logic [9:0] BallX, BallY, Ball_sizeX, Ball_sizeY, DrawX, DrawY,
							  input logic [9:0] BallX_two, BallY_two, BallX_three, BallY_three, BallX_four, BallY_four
							  input logic Note_END_one, Note_END_two, Note_END_three, Note_END_four,
                       output logic [7:0]  Red, Green, Blue );
    
    logic ball_on1, ball_on2, ball_on3, ball_on4;

    int DistX, DistY, SizeX, SizeY;
	 assign DistX = DrawX - BallX;
    assign DistY = DrawY - BallY;
    assign SizeX = Ball_sizeX;
	 assign SizeY = Ball_sizeY;
	  
    always_comb
    begin:Ball_on_proc1
        if ( DrawX >= BallX && DrawX < BallX + Ball_sizeX ) 
            ball_on1 = 1'b1;
        else 
            ball_on1 = 1'b0;
    end 

	 always_comb
    begin:Ball_on_proc2
        if ( DrawX >= BallX_two && DrawX < BallX_two + Ball_sizeX ) 
            ball_on2 = 1'b1;
        else 
            ball_on2 = 1'b0;
    end 
	 
	 always_comb
    begin:Ball_on_proc3
        if ( DrawX >= BallX_three && DrawX < BallX_three + Ball_sizeX ) 
            ball_on3 = 1'b1;
        else 
            ball_on3 = 1'b0;
    end 
	 
	 always_comb
    begin:Ball_on_proc4
        if ( DrawX >= BallX_four && DrawX < BallX_four + Ball_sizeX ) 
            ball_on4 = 1'b1;
        else 
            ball_on4 = 1'b0;
    end 
	 
    always_comb
    begin:RGB_NoteOne
        if ((ball_on1 == 1'b1)) 
        begin 
            Red = 8'h00;
            Green = 8'hff;
            Blue = 8'hff;
        end       
        else if (Note_END_one == 1'b1)
        begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end      
		  else
		  begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end    
    end 
 
    always_comb
    begin:RGB_NoteTwo
        if ((ball_on2 == 1'b1)) 
        begin 
            Red = 8'h00;
            Green = 8'hff;
            Blue = 8'hff;
        end       
        else if (Note_END_two == 1'b1)
        begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end      
		  else
		  begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end    
    end 
	
    always_comb
    begin:RGB_NoteThree
        if ((ball_on3 == 1'b1)) 
        begin 
            Red = 8'h00;
            Green = 8'hff;
            Blue = 8'hff;
        end       
        else if (Note_END_three == 1'b1)
        begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end      
		  else
		  begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end    
    end 
	
    always_comb
    begin:RGB_NoteFour
        if ((ball_on4 == 1'b1)) 
        begin 
            Red = 8'h00;
            Green = 8'hff;
            Blue = 8'hff;
        end       
        else if (Note_END_four == 1'b1)
        begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end      
		  else
		  begin 
            Red = 8'h4f - DrawX[9:3];
            Green = 8'h00;
            Blue = 8'h44; 
        end    
    end 	
endmodule
