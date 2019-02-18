module notes(input logic Reset, Clk, 
             input logic [7:0] keycode,
				 output logic pixel_clk, blank, sync, hs, vs, 
				 output logic [7:0] Red, Green, Blue);

						  
parameter bpm = 310; //155*2 = 310
parameter ppb = 64; // pixels per beat
logic clk25; //25 Mhz clk for use in this module
	 
logic [12:0] current_note_addr; //current note read from song data
logic [3:0] current_note_data; //current note data from song data
logic start; //indicator of song start

logic current_note_data0;
logic current_note_data1;
logic current_note_data2;
logic current_note_data3;
logic end_of_note1, end_of_note2, end_of_note3, end_of_note4;

assign current_note_data1 = current_note_data[3];
assign current_note_data2 = current_note_data[2];
assign current_note_data3 = current_note_data[1];
assign current_note_data4 = current_note_data[0];

songdata hidan_no_aria(.clk(clk), .addr(current_note_addr), .data(current_note_data));

logic [16:0] beatcount; //beat counter to track song progress
logic [32:0] counter; //seperate counter to track in terms of monitor cycles
logic [8:0] pixelcounter; //counter to track vertical pixel movement, 9 bits since Y resolution is 480 pixels

logic [9:0] Note_oneX, Note_oneY, Note_one_sizeX, Note_one_sizeY, DrawX, DrawY;
logic [9:0] Note_twoX, Note_twoY, Note_two_sizeX, Note_two_sizeY;
logic [9:0] Note_threeX, Note_threeY, Note_three_sizeX, Note_three_sizeY;
logic [9:0] Note_fourX, Note_fourY, Note_four_sizeX, Note_four_sizeY;
	

always_ff @ (posedge Clk or posedge Reset ) //This cuts the 50 Mhz clock in half to generate a 25 MHz pixel clock  
begin 
    if (Reset) 
        clk25 <= 1'b0;
    else 
        clk25 <= ~ (clk25);
end
					
always @(posedge clk25) begin  //using 25Mhz clock to control beat of song to sync with monitor
		
		if (beatcount == 250) begin  //checking for end of song couple measures after song end
				start = 0;
		end 

		if (Reset && beatcount < 236) 
				start = ~start;	
		else if (Reset) begin
				beatcount = 0;
				start = 0;
				counter = 0;
				pixelcounter = ppb/2;
		end
			
		counter = counter + start;
		if (counter == 25000000*60/bpm/ppb) begin
				counter = 0;
				pixelcounter = pixelcounter +1; //moving everything down by one pixel
				if (pixelcounter == ppb) begin
						pixelcounter = 0; //reset pixelcounter for this beat/cycle
						beatcount = beatcount + 1; //increment the beat by one
				end
		end
		
		current_note_addr = beatcount; //need to recheck this, temporary line to implement a current_note_addr
end
					
note note_one(.Reset(Reset), .frame_clk(vs), .note_data(current_note_data1), .columndata(2'b00),
				  .NoteX(Note_oneX), .NoteY(Note_oneY), .NoteSX(Note_one_sizeX), .NoteSY(Note_one_sizeY), 
				  .keycode(keycode), .NoteEND(end_of_note1));
				  
note note_two(.Reset(Reset), .frame_clk(vs), .note_data(current_note_data2), .columndata(2'b01),
			     .NoteX(Note_twoX), .NoteY(Note_twoY), .NoteSX(Note_two_sizeX), .NoteSY(Note_two_sizeY), 
				  .keycode(keycode), .NoteEND(end_of_note2));
				  
note note_three(.Reset(Reset), .frame_clk(vs), .note_data(current_note_data3), .columndata(2'b10),
				    .NoteX(Note_threeX), .NoteY(Note_threeY), .NoteSX(Note_three_sizeX), .NoteSY(Note_three_sizeY), 
					 .keycode(keycode), .NoteEND(end_of_note3));
					 
note note_four(.Reset(Reset), .frame_clk(vs), .note_data(current_note_data4), .columndata(2'b11),
				   .NoteX(Note_fourX), .NoteY(Note_fourY), .NoteSX(Note_four_sizeX), .NoteSY(Note_four_sizeY), 
					.keycode(keycode), .NoteEND(end_of_note4));
					
color_mapper clr_map(.BallX(Note_oneX), .BallY(Note_oneY), 
							.BallX_two(Note_twoX), .BallY_two(Note_twoY), 
							.BallX_three(Note_threeX), .BallY_three(Note_threeY), 
							.BallX_four(Note_fourX), .BallY_four(Note_fourY), 
							.DrawX(DrawX), .DrawY(DrawY), 
							.Ball_sizeX(Note_one_size), .Ball_sizeY(Note_one_sizeY), 
							.Red(Red), .Green(Green), .Blue(Blue), 
							.Note_END_one(end_of_note1), .Note_END_two(end_of_note2), .Note_END_three(end_of_note3), .Note_END_four(end_of_note4));
//current implementation of this color mapper only supports one note column, need to expand to account for all four columns of notes
							  			  
vga_controller vga(.Clk(Clk),             // 50 MHz clock
                   .Reset(Reset),         // reset signal
                   .hs(hs),               // Horizontal sync pulse.  Active low
						 .vs(vs),               // Vertical sync pulse.  Active low
						 .pixel_clk(pixel_clk), // 25 MHz pixel clock output
						 .blank(blank),         // Blanking interval indicator.  Active low.
						 .sync(sync),           // Composite Sync signal.  Active low.  We don't use it in this lab,
														//   but the video DAC on the DE2 board requires an input for it.
						 .DrawX(DrawX),         // horizontal coordinate
						 .DrawY(DrawY) );

endmodule
