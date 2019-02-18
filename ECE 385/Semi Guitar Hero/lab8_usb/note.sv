module  note ( input Reset, frame_clk, note_data,
               output [9:0]  NoteX, NoteY, NoteSX, NoteSY, 
					output NoteEND,
					input [7:0] keycode
					input [1:0] columndata);
    
    logic [9:0] Note_X_Pos, Note_X_Motion, Note_Y_Pos, Note_Y_Motion, Note_SizeX, Note_SizeY;
	 logic Note_end;
	 
    parameter [9:0] Note_X_Start=40;	 // Starting position on the X axis, 160 pixels per note, (160 - width of note block)/2
    parameter [9:0] Note_Y_Start=0;     // Starting position on the Y axis, starts at bottom of screen
    parameter [9:0] Note_X_Min=0;       // Leftmost point on the X axis
    parameter [9:0] Note_X_Max=639;     // Rightmost point on the X axis
    parameter [9:0] Note_Y_Min=0;       // Topmost point on the Y axis
    parameter [9:0] Note_Y_Max=479;     // Bottommost point on the Y axis
    parameter [9:0] Note_X_Step=2;      // 
    parameter [9:0] Note_Y_Step=2;      // Step size needs to be in sync with pixels per beat, going to set to arbitrary value for now, needs experimenting

    assign Note_SizeX = 70;  // Width of note block
	 assign Note_SizeY = 20;  // Height of note block
   
always_ff @ (posedge frame_clk)  // removed or posedge Reset, because reset not used in this module to reset the notes/songdata
begin: Move_Note
		if (note_data == 1'b1) begin
				Note_Y_Pos <= Note_Y_Start;
				Note_X_Pos <= (Note_X_Start + 70*columndata)
				Note_Y_Motion <= ( ~ (Note_Y_Step) + 1'b1);		
		end		
		else if ( (Note_Y_Pos - Note_SizeY) <= Note_Y_Max ) begin //Note hits the top edge, it needs to dissapear
				Note_end = 1'b1;
				Note_X_Pos = 1'b0;
				Note_Y_Pos = 1'b0;
		end
		
		Note_Y_Pos <= (Note_Y_Pos + Note_Y_Motion); //Update note position
				
end		

assign NoteX = Note_X_Pos;
assign NoteY = Note_Y_Pos;

assign NoteSX = Note_SizeX;
assign NoteSY = Note_SizeY;

assign Note_END = Note_end;

endmodule
