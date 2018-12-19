
public class PageRankTest {
	private int nodesNumber;
	private String graphName;
	private String dataPath;
	private double THRESHOLD=0.0001;
	private double D_VALUE=0.15; 

	public static void main(String[] args) {
		PageRankTest test0 = new PageRankTest(6,"graph_0");
		test0.start();
		
		/*
		PageRankTest test1 = new PageRankTest(6,"graph_1");
		PageRankTest test2 = new PageRankTest(5,"graph_2");
		PageRankTest test3 = new PageRankTest(4,"graph_3");
		PageRankTest test4 = new PageRankTest(7,"graph_4");
		PageRankTest test5 = new PageRankTest(469,"graph_5");
		PageRankTest test6 = new PageRankTest(1228,"graph_6");
		PageRankTest test7 = new PageRankTest(20,"data_result");
		PageRankTest test8 = new PageRankTest(20,"association_result");
		
		test1.start();
		
		test2.start();
		test3.start();
		
		 
		 
		test4.start();
		
		test5.start();
		
		test6.start();
		
		
		test7.start();
		test8.start();
		*/
		
		PageRankTest test1 = new PageRankTest(6,"graph_1");
		test1.start();
	}
	
	public PageRankTest(int totalNodeNumber ,String testGraphName){
		nodesNumber = totalNodeNumber;
		graphName = testGraphName;
		dataPath = "./dataset/"+graphName+".txt";
	}
	
	public void start(){
		 FileParser fp = new FileParser(dataPath,nodesNumber);
		 double time1 ,time2;
		 
	        try {
	        	fp.readData();
	        	int[][] adjMatrix =fp.getAdjacentMatrix();
	            PageRank pr = new PageRank(nodesNumber,adjMatrix,THRESHOLD,D_VALUE);
	            
	            time1= System.nanoTime();
	            pr.calc();
	            time2= System.nanoTime();
	            
	            pr.sortPageRank();
	            //pr.writePageRankToFile(graphName);
	            System.out.println(graphName +"," +(time2-time1)/1000000);
	        }catch (Exception e) {
	        	System.out.println(e.getMessage());
	        }
	}
	

}
