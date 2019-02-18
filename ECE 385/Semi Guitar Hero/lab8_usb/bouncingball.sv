module bouncingball(input logic Reset, Clk, 
                    input logic [7:0] keycode,
						  output logic pixel_clk, blank, sync, hs, vs, 
						  output logic [7:0] Red, Green, Blue);

						  
						  
logic [9:0] BallX, BallY, Ball_size, DrawX, DrawY;	
	
		  

ball Ball1 (.Reset(Reset), .frame_clk(vs),
      .BallX(BallX), .BallY(BallY), .BallS(Ball_size),
	   .keycode(keycode));
					
					
color_mapper clr_map1( .BallX(BallX), .BallY(BallY), .DrawX(DrawX), .DrawY(DrawY), .Ball_size(Ball_size),
               .Red(Red), .Green(Green), .Blue(Blue));
							  
							  
							  
vga_controller vga(.Clk(Clk),       // 50 MHz clock
                .Reset(Reset),     // reset signal
                .hs(hs),        // Horizontal sync pulse.  Active low
	             .vs(vs),        // Vertical sync pulse.  Active low
		          .pixel_clk(pixel_clk), // 25 MHz pixel clock output
				    .blank(blank),     // Blanking interval indicator.  Active low.
					 .sync(sync),      // Composite Sync signal.  Active low.  We don't use it in this lab,
												             //   but the video DAC on the DE2 board requires an input for it.
					.DrawX(DrawX),     // horizontal coordinate
					.DrawY(DrawY) );
					
					
endmodule