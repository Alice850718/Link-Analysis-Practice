import java.util.*;
import java.io.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class FileParser {
	private int nodeNumber;
	private String dataPath;
	private int[][] adjacentMatrix;
	private int[] outdegreeVector;
	
	
	public FileParser (String dataPath,int nodeNumber) {
		this.dataPath = dataPath;
		this.nodeNumber = nodeNumber;
	}
	
	public void readData() throws Exception{
		
		if( DataPathIsValid() && nodeNumberIsValid() ) {	
			FileReader fr = new FileReader(dataPath);
			BufferedReader br = new BufferedReader(fr);
			adjacentMatrix = new int[nodeNumber+1][nodeNumber+1];
			outdegreeVector = new int[nodeNumber+1];		
			
			for(int row=0;row < nodeNumber ; row++) {
				Arrays.fill(adjacentMatrix[row],0 );
			}		
			Arrays.fill(outdegreeVector, 0);
			
	        while (br.ready()) {
	        	String line = br.readLine();
	        	String edge[] =line.split(",");
	        	int origin = Integer.parseInt(edge[0]);
	        	int terminus = Integer.parseInt(edge[1]);
	        	adjacentMatrix[origin][terminus] =1;
	        	outdegreeVector[origin]=outdegreeVector[origin]+1;	        	
	        }             
	        fr.close();
		}
	}
	
	private boolean DataPathIsValid() {
		return !dataPath.isEmpty();
	}
	
	private boolean nodeNumberIsValid() {
		return nodeNumber >0; 
	}
	
		
	public int[][] getAdjacentMatrix() {
		return adjacentMatrix;
	}
	
	public int[] getOutdegreeVector() {
		return outdegreeVector;
	}
	

}
