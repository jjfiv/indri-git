package edu.umass.cs.indri.ui;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import edu.umass.cs.indri.*;

/**
   User interface for indri retrieval.
   @author David Fisher
   @version 1.0
*/

public class RetUI extends JPanel implements ActionListener {
    /** File chooser starting in current directory */
    private final JFileChooser fc = 
	new JFileChooser(System.getProperties().getProperty("user.dir"));
	
    /** About the GUI. */
    private final static String aboutText = "Indri Retrieval UI 1.0\n" +
	"Copyright (c) 2004 University of Massachusetts\n";
	
    /** Help file for the application */
    private final static String helpFile = "properties/IndriRetrieval.html";
    /** Frame for help window */
    private JFrame helpFrame;
	
    /**
     * Search and Clear buttons
     */
    JButton go, stop;
    /**
     * Status line output
     */
    JLabel status;
    /**
     * Progress indicator output
     */
    JLabel progress;
    /**
     * Query input
     */
    JTextField query;
    /**
     * Number of documents to retrieve input
     */
    JTextField numDocs;
    /**
     * Retrieved documents table
     */
    JTable answerAll;
    /**
     * Frame for showing document text and query tree.
     */
    JFrame docTextFrame;
    /**
     * Document text pane.
     */
    JTextPane docTextPane;
    /**
     * Query tree display
     */
    JTree docQueryTree;
    /**
     * Indri query environment for retrieval.
     */
    QueryEnvironment env;
    /**
     * Map containing query annotations for highlighting.
     */
    Map annotations = null;
    /**
     * Result holder for query retrieval.
     */
    QueryAnnotation results = null;
    /**
     * Result holder for document scored extents
     */
    ScoredExtentResult[] scored = null;
    /**
     * Result holder for parsed documents
     */
    //    ParsedDocument[] docs = null;
    ParsedDocument currentParsedDoc = null;
    /**
     * Result holder for currently selected document id
     */
    int currentDocId = 0;
    
    /**
     * Result holder for document external ids
     */
    String [] names = null;
    /**
     * Result holder for document internal ids
     */
    int [] docids = null;
    /**
     * Index into result holders for the current document
     */
    int currentDoc = -1;
    /**
     * Maximum number of documents to retrieve for a query
     */
    int maxDocs = 10;
    /**
     * Flag for whether or not the query environment has
     * been initialized.
     */
    boolean envInit = false;
	
    /** Enable multiple indexes */
    private JList documents;
    /** hold the file names */
    private DefaultListModel documentsModel;
	
    /** Enable multiple indexes */
    private JList indexes;
    /** hold the file names */
    private DefaultListModel indexesModel;
    /** colors */
    private static final Color lightYellow = new Color(255, 255, 224);
    /** colors */
    private static final Color navyBlue = new Color(0, 0, 128);
    /** colors */
    private static final Color linen = new Color(250, 240, 230);
	
    /**
     * Initialize the query environment and indexes model.
     */
    public RetUI () {
	env = new QueryEnvironment();
	indexesModel = new DefaultListModel();
    }
	
    /**
     * Create the gui elements, including the help frame.
     */
    public void init() {
	// initialize help
	makeHelp();
	Font myFont = new Font("SansSerif", Font.BOLD, 14);
	JPanel p;
	Box myBox = Box.createVerticalBox();
		
	setBackground(lightYellow);
	setForeground(navyBlue);
	// add tool tips
	go = new JButton("Search");
	go.addActionListener(this);
	go.setToolTipText("Search the collection");
	stop = new JButton("Clear");
	stop.addActionListener(this);
	stop.setToolTipText("Clear the display");
	status = new JLabel("Open an index or server", null, JLabel.CENTER);
	status.setFont(myFont);
	status.setForeground(Color.red);
	status.setBackground(lightYellow);
	progress = new JLabel("            ", null, JLabel.LEFT);
	progress.setFont(myFont);
	progress.setForeground(Color.red);
	progress.setBackground(lightYellow);	
		
	Font anotherFont = new Font("SansSerif", Font.BOLD, 14);
	query = new JTextField(30);
	query.setBackground(lightYellow);
	query.setForeground(navyBlue);
	query.addActionListener(this);
	query.setFont(anotherFont);
	query.setToolTipText("Enter a query");
	p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(query, BorderLayout.CENTER);
	JLabel x = new JLabel("Enter your query:          ", null, JLabel.CENTER);
	myFont = new Font("SansSerif", Font.BOLD + Font.ITALIC, 14);
	x.setFont(myFont);
	x.setForeground(navyBlue);
	x.setBackground(lightYellow);
	p.add(x, BorderLayout.WEST);
	p.setPreferredSize(new Dimension(600, 20));
	//	p.setMaximumSize(new Dimension(600, 20));
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
	myBox.add(p);
		
	p = new JPanel();
	p.setLayout(new BorderLayout());
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
		
	x = new JLabel("Number of documents: ", null, JLabel.CENTER);
	x.setFont(myFont);
	x.setForeground(navyBlue);
	x.setBackground(lightYellow);
		
	p.add(x, BorderLayout.WEST);
	numDocs = new JTextField(5);
	numDocs.setBackground(lightYellow);
	numDocs.setForeground(navyBlue);
	numDocs.setFont(anotherFont);
	numDocs.setText("" + maxDocs);
	numDocs.setToolTipText("Enter maximum number of documents to retrieve");
	p.add(numDocs, BorderLayout.CENTER);
		
	// Nasty Hack!
	x = new JLabel("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", null,
		       JLabel.CENTER);
	x.setFont(myFont);
	x.setForeground(lightYellow);
	x.setBackground(lightYellow);
	p.add(x, BorderLayout.EAST);
	p.setPreferredSize(new Dimension(600, 20));
	//	p.setMaximumSize(new Dimension(600, 20));
	myBox.add(p);
		
	p = new JPanel();
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
		
	p.setLayout(new BorderLayout());
	x = new JLabel("Database(s):                  ", null, JLabel.CENTER);
	x.setFont(myFont);
	x.setForeground(navyBlue);
	x.setBackground(lightYellow);
	p.add(x, BorderLayout.WEST);
		
	indexes = new JList(indexesModel);
	indexes.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
	String fill = "12345678901234567890123456789012345678901234567890";
	indexes.setPrototypeCellValue(fill);
	indexes.setVisibleRowCount(3);
	indexes.setForeground(navyBlue);
	indexes.setBackground(lightYellow);
	JScrollPane listScrollPane = new JScrollPane(indexes);
		
	p.add(listScrollPane, BorderLayout.CENTER);
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
	p.setPreferredSize(new Dimension(600, 60));
	//	p.setMaximumSize(new Dimension(600, 60));
	myBox.add(p);
		
	p = new JPanel();
	p.add(go);
	p.add(stop);
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
	JPanel p1 = new JPanel();
	p1.setLayout(new BorderLayout());
	p1.add(progress, BorderLayout.NORTH);
	p1.add(p, BorderLayout.CENTER);
	p1.add(status, BorderLayout.SOUTH);
	p1.setPreferredSize(new Dimension(600, 80));
	//	p1.setMaximumSize(new Dimension(600, 80));
	p1.setForeground(navyBlue);
	p1.setBackground(lightYellow);
	myBox.add(p1);
		
	p = new JPanel();
	p.setLayout(new BorderLayout());
	p.setForeground(navyBlue);
	p.setBackground(lightYellow);
	anotherFont = new Font("SansSerif", Font.PLAIN, 18);
	makeDocTextFrame();
	answerAll = makeDocsTable();
	answerAll.setFont(anotherFont);
	answerAll.setBackground(linen);
	answerAll.setForeground(Color.black);
	answerAll.setPreferredSize(new Dimension(600, 400));
	answerAll.setPreferredScrollableViewportSize(new Dimension(600, 400));
	answerAll.setToolTipText("Select an entry to view the document");
	JScrollPane scroll = new JScrollPane(answerAll);
	scroll.setPreferredSize(new Dimension(600, 400));
	//	scroll.setMinimumSize(new Dimension(600, 200));
	//	scroll.setMaximumSize(new Dimension(600, 400));
	scroll.setDoubleBuffered(true);
	p.add(scroll, BorderLayout.CENTER);
	p.setPreferredSize(new Dimension(600, 400));
	//	p.setMinimumSize(new Dimension(600, 400));
	myBox.add(p);
	add(myBox);
	query.setEnabled(envInit);
	go.setEnabled(envInit);
    }
	
    /* (non-Javadoc)
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
	// clear any error message.
	error("                  ");
	String act = e.getActionCommand();
	if (act.equals("Search") || e.getSource() == query) {
	    // query to run
	    runQuestion();
	} else if (act.equals("Clear")) {
	    query.setText("");
	    status.setText("");
	    DocsTableModel m = (DocsTableModel)answerAll.getModel();
	    m.clear();
	    // clear the docs frame
	    docTextPane.setText("");
	    docTextFrame.setTitle("Document");
	    // clear the query
	    docQueryTree.setModel(new DefaultTreeModel(new DefaultMutableTreeNode("Query")));
	    query.requestFocus();
	    // need a remove index/server. 
	} else if (act.equals("Add Index")) {
	    status.setText("Opening...");
	    openIndex();
	    query.setEnabled(envInit);
	    go.setEnabled(envInit);
	    query.requestFocus();
	    status.setText("");
	} else if (act.equals("Add Server")) {
	    status.setText("Opening...");
	    openServer();
	    query.setEnabled(envInit);
	    go.setEnabled(envInit);
	    query.requestFocus();
	    status.setText("");
	} else if (act.equals("Remove Selected Index")) {
	    status.setText("Removing...");
	    removeIndex();
	    query.setEnabled(envInit);
	    go.setEnabled(envInit);
	    query.requestFocus();
	    status.setText("");
	} else if (act.equals("About")) 	{
	    JOptionPane.showMessageDialog(this, aboutText, "About", 
					  JOptionPane.INFORMATION_MESSAGE,
					  null);
	} else if (act.equals("Help")) {
	    // pop up a help dialog
	    helpFrame.setVisible(true);
	} else if (act.equals("Exit")) {
	    System.exit(0);
	}
    }
	
	
    /**
     * Remove the selected index from the list. Close and
     * reopen the query environment with any remaining indexes.
     */
    public void removeIndex() {
	// index is selected in the list.
	int idx = indexes.getSelectedIndex();
	if (idx == -1) // no selection, we're done
	    return;
	indexesModel.remove(idx);
	// close the env, reopen with remaining indexes.
	env.close();
	envInit = false;
	// iterate over elements. if a host, add a server, else an index.
	Enumeration elts = indexesModel.elements();
	while (elts.hasMoreElements()) {
	    try {
		String s = (String)elts.nextElement();
		if (s.startsWith("Server: ")){
		    s = s.substring(8);
		    env.addServer(s);
		} else {
		    env.addIndex(s);
		}
		envInit = true;
	    } catch (Exception e){
		error(e.toString());
	    }
	}
    }
	
    /**
     * Present a file chooser to select an index to open.
     * Open the index in the query environment.
     */
    public void openIndex() {
	fc.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
	int returnVal = fc.showOpenDialog(this);
	if (returnVal == JFileChooser.APPROVE_OPTION) {
	    File file = fc.getSelectedFile();
	    String index = file.getAbsolutePath();
	    indexesModel.addElement(index);
	    try {
		env.addIndex(index);
		envInit = true;
	    } catch (Exception e){
		// remove the offending elt.
		indexesModel.removeElement(indexesModel.lastElement());
		error(e.toString());
	    }
	}
    }
	
    /**
     * Present an input dialog to obtain a hostname and optional
     * port number. Open the given server in the query environment.
     */
    public void openServer() {
	//simple text entry dialog for server name/port
	String index = JOptionPane.showInputDialog("Enter a server name, with optional port number (host[:portnum]).");
	indexesModel.addElement("Server: " + index);
	try {	    
	    env.addServer(index);
	    envInit = true;
	} catch (Exception e){
	    // remove the offending elt.
	    indexesModel.removeElement(indexesModel.lastElement());
	    error(e.toString());
	}
    }
	
    /**
     * Run the given query in a separate thread.
     */
    public void runQuestion() {
	Runnable r = new Runnable() {
		public void run() {
		    String question = query.getText();
		    // don't try to run an empty query.
		    if (question.equals("")) return;
		    status.setText("working...");
		    Thread blink = blinker("working", "done.");
		    DocsTableModel m = (DocsTableModel)answerAll.getModel();
		    m.clear();
		    // clear the docs frame
		    docTextPane.setText("");
		    docTextFrame.setTitle("Document");
		    // clear the query
		    docQueryTree.setModel(new DefaultTreeModel(new DefaultMutableTreeNode("Query")));
		    try {
			maxDocs = Integer.parseInt(numDocs.getText());
		    } catch (NumberFormatException e){
		    }
		    // results have extents and matches.
		    try {
			results = env.runAnnotatedQuery( question, maxDocs );
			scored = results.getResults();
			names = env.documentMetadata( scored, "docno" );
			// don't populate this table, too much memory wasted.
			//			docs = env.documents(scored);
			docids = new int[scored.length];
			for (int j = 0; j < scored.length; j++)
			    docids[j] = scored[j].document;
			m.resize(scored.length); 
			// initialize scored docs table
			// could put the internal docid into the table,
			// rather than using the separate docids map.
			// could use clever tooltips too (to expand title).
			for( int i = 0; i < scored.length; i++ ) {
			    m.setValueAt(i, scored[i].score, names[i],
					 scored[i].begin, scored[i].end);
			}
			// initialize query tree view for doc text frame
			// keys query node names (as inserted in QueryTree).
			// get root node
			QueryAnnotationNode query = results.getQueryTree();
			annotations = results.getAnnotations();
			// update the JTree
			DefaultTreeModel tree = makeTreeModel(query);
			docQueryTree.setModel(tree);
			//
		    } catch (Exception e) {
			System.err.println("Caught: +" + e.toString());
					
			e.printStackTrace();
		    }
		    blinking = false;
		    blink.interrupt();
		}
	    };
	Thread t = new Thread(r);
	t.start();
    }    

    /** Create the frame that shows the help file and render the html.
	Saves time when the user opens the help.
    */
    private void makeHelp() {
	java.net.URL helpURL = RetUI.class.getResource(helpFile);
	JTextPane help = new JTextPane();
		
	//Create and set up the window.
	helpFrame = new JFrame("Indri Retrieval UI Help");
	help.setPreferredSize(new Dimension(650, 400));
	help.setEditable(false);
	help.addHyperlinkListener(new DocLinkListener());
	JScrollPane scroller =
	    new JScrollPane(help, 
			    JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			    JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
	try {
	    help.setPage(helpURL);
	} catch (IOException ex) {
	    help.setText("Help file unavailable.");
	}

	helpFrame.getContentPane().add(scroller, BorderLayout.CENTER);
	helpFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
	//	helpFrame.setIconImage(createImageIcon(iconFile).getImage());
	helpFrame.pack();
    }
	
	
    /**
     * Make the frame for displaying highlighted retrieved documents
     * with both document text and the query tree.
     */
    private void  makeDocTextFrame() {
	docTextFrame = new JFrame("Document");
	docTextPane = new JTextPane();
	docTextPane.setEditable(false);
	docTextPane.setFont(new Font("SansSerif", Font.PLAIN, 14));
	docTextPane.setBackground(linen);
	docTextPane.setForeground(Color.black);
	docTextPane.setPreferredSize(new Dimension(550, 350));
		
	JScrollPane scroll = new JScrollPane(docTextPane);
	scroll.setPreferredSize(new Dimension(600, 350));
	//	scroll.setMinimumSize(new Dimension(600, 200));
	//	scroll.setMaximumSize(new Dimension(600, 350));
	scroll.setDoubleBuffered(true);
	JPanel p = new JPanel();
	p.setLayout(new BorderLayout());
	p.add(scroll, BorderLayout.SOUTH);
	// query pane table (JTree?);
	// make this afresh for each query?
	DefaultMutableTreeNode top = new DefaultMutableTreeNode("Query");
	docQueryTree = new JTree(top);
	docQueryTree.addTreeSelectionListener(new QueryTreeListener());
	docQueryTree.setPreferredSize(new Dimension(550, 180));
	//	docQueryTree.setMaximumSize(new Dimension(550, 180));
	JScrollPane treeView = new JScrollPane(docQueryTree);
	treeView.setPreferredSize(new Dimension(600, 180));
	//	treeView.setMaximumSize(new Dimension(600, 180));
	//	treeView.setMinimumSize(new Dimension(600, 180));
	treeView.setDoubleBuffered(true);
	p.add(treeView, BorderLayout.NORTH);
	p.setPreferredSize(new Dimension(600, 600));
	docTextFrame.getContentPane().add(p);
	docTextFrame.setSize(600,600);
	docTextFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
    }
	
    /**
     * Populate the document text frame with the selected document.
     * Highlight the document based on query matches.
     */
    public void getDocText() {
	// get the selected index from the table.
	int row = answerAll.getSelectionModel().getMinSelectionIndex();
	// no selection.
	if (row == -1) return;

	// get the doc text
	TableModel m = answerAll.getModel();
	String name = (String) m.getValueAt(row, 1);
	docTextFrame.setTitle(name);
	currentDocId = docids[row]; // internal docid
	// get the parsed document
	int [] ids = new int[1];
	ids[0] = currentDocId;
	
	ParsedDocument[] docs = env.documents(ids);
	currentParsedDoc = docs[0];
	String myDocText = currentParsedDoc.text;
	//	String myDocText = docs[row].text;
	// insert into doc text pane
	docTextPane.setContentType("text/plain");
	docTextPane.setText(myDocText);
	// reset caret to start of doc text.
	docTextPane.setCaretPosition(0);
	// insert the matches markup
	DefaultTreeModel tree = (DefaultTreeModel) docQueryTree.getModel();
	DefaultMutableTreeNode root = (DefaultMutableTreeNode) tree.getRoot();
	highlight(root);
	//	docTextFrame.setLocationRelativeTo(this);
	docTextFrame.setVisible(true);
    }    
	
    //rework this to separate clear from single node highlight
    /**
     * Insert highlighting markup into a styled document, 
     * starting from the given node of the query tree.
     * If the node is the root of the tree, recurse through the
     * entire tree.
     * @param query The query tree node to highlight
     */
    private void highlight(DefaultMutableTreeNode query) {
	StyledDocument myDoc = docTextPane.getStyledDocument();
	MutableAttributeSet highlight = new SimpleAttributeSet();
	// iterate over matches.
	DefaultTreeModel tree = (DefaultTreeModel) docQueryTree.getModel();
	DefaultMutableTreeNode root = (DefaultMutableTreeNode) tree.getRoot();
	// clear any attributes.
	clearHighlight(myDoc, highlight);
	// set highlighting colors
	StyleConstants.setForeground(highlight,linen);
	StyleConstants.setBackground(highlight,Color.red);
	boolean replace = false;

	if (query == root) {
	    Enumeration queryNodes = root.breadthFirstEnumeration();
	    while (queryNodes.hasMoreElements()) {
		DefaultMutableTreeNode tn = (DefaultMutableTreeNode) queryNodes.nextElement();
		UIQueryNode q = (UIQueryNode) tn.getUserObject();
		QueryAnnotationNode node = q.getNode();
		// only highlight raw scorer nodes when doing full tree.
		if (node.type.equals("RawScorerNode")) {
		    highlight(q, myDoc, highlight);
		}
		
	    } 
	} else {
		highlight((UIQueryNode) query.getUserObject(), myDoc, 
			  highlight);
	}
    }

    /**
     * Clear all highlighting markup in a styled document.
     * @param myDoc The StyledDocument to highlight
     * @param highlight The highlighting attributes
     */
    private void clearHighlight(StyledDocument myDoc, 
				MutableAttributeSet highlight) {
	// clear any attributes.
	myDoc.setCharacterAttributes(myDoc.getStartPosition().getOffset(), 
				     myDoc.getEndPosition().getOffset(), 
				     highlight, true);
    }
	
    /**
     * Highlight the current document in the document text pane
     * with respect to the given query node.
     * @param q The query node to highlight extents for
     * @param myDoc The StyledDocument to highlight
     * @param highlight The highlighting attributes
     */
    private void highlight(UIQueryNode q, StyledDocument myDoc, 
			   MutableAttributeSet highlight) {
	boolean replace = false;
	// iterate over matches.
	// keys query node names (as inserted in QueryTree).
	QueryAnnotationNode node = q.getNode();
	String name = node.name;
	ScoredExtentResult[] extents = (ScoredExtentResult[])annotations.get(name);
	if (extents != null) {
	    for (int i = 0; i < extents.length; i++) {		
		if (currentDocId == extents[i].document) {
		    int start = currentParsedDoc.positions[extents[i].begin].begin;
		    int end = currentParsedDoc.positions[extents[i].end-1].end;
		    myDoc.setCharacterAttributes(start, (end - start), 
						 highlight, replace);
		}
	    }
	}
    }
	
    /**
     * Create a TreeModel from a QueryAnnotationNode.
     * @param query The QueryAnnotationNode to use as the root of the tree
     * @return A DefaultTreeModel rooted in <code>query</code>
     */
    private DefaultTreeModel makeTreeModel(QueryAnnotationNode query) {
	DefaultMutableTreeNode top = makeChildNode(query);
	DefaultTreeModel tree = new DefaultTreeModel(top);
	return tree;
    }

    /**
     * Make a DefaultMutableTreeNode out of a QueryAnnotationNode.
     * Recursively insert all children of the query into the tree.
     * @param query The QueryAnnotationNode to insert into the tree.
     * @return a DefaultMutableTreeNode for this query.
     */
    private DefaultMutableTreeNode makeChildNode(QueryAnnotationNode query) {
	UIQueryNode q = new UIQueryNode(query);
	DefaultMutableTreeNode parent = new DefaultMutableTreeNode(q);
	DefaultMutableTreeNode child;
	for (int i = 0; i < query.children.length; i++) {
	    child = makeChildNode(query.children[i]);
	    parent.add(child);
	}
	return parent;
    }
    /**
     * Listener for the query tree to update highlighting when
     * the query node selection changes.
     * @author David Fisher
     *
     */
    private class QueryTreeListener implements TreeSelectionListener  {
		
	/* (non-Javadoc)
	 * @see javax.swing.event.TreeSelectionListener#valueChanged(javax.swing.event.TreeSelectionEvent)
	 */
	public void valueChanged(TreeSelectionEvent e) 
	{
	    if (e.isAddedPath()) {
		TreePath path = e.getPath();
		DefaultMutableTreeNode node = 
		    (DefaultMutableTreeNode)(path.getLastPathComponent());
		highlight(node);
	    }
	}
    }
	
    /**
     * Listener on the retrieved documents table that enables populating
     * the document text frame with the selection of a document.
     * @author David Fisher
     */
    private class DocTableListener implements ListSelectionListener  {
		
	/* (non-Javadoc)
	 * @see javax.swing.event.ListSelectionListener#valueChanged(javax.swing.event.ListSelectionEvent)
	 */
	public void valueChanged(ListSelectionEvent e) {
	    // cleared selection?
	    getDocText();
	}
    }
	
    /**
     * Adapter for QueryAnnotationNode for populating the
     * query tree display.
     * @author David Fisher
     */
    private class UIQueryNode {
		
	/**
	 * Wrap a node.
	 * @param n The node to wrap.
	 */
	public UIQueryNode(QueryAnnotationNode n)
	{
	    node = n;
	}
		
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
	    String retval = node.queryText + " (" + node.type + ")";
	    return retval;
	}
	/**
	 * Unwrap the node
	 * @return the wrapped QueryAnnotationNode.
	 */
	public QueryAnnotationNode getNode() { return node; }
	QueryAnnotationNode node;
    }
	
    /**
     * Semaphore for the blinker thread
     */
    private volatile boolean blinking = false;
	
    /**
     * Blink a pair of messages on the status line
     * @param s1 The first message to display
     * @param s2 The second message to display
     * @return The Thread doing the blinking
     */
    private Thread blinker(final String s1, final String s2) {
	Thread blink;
	blink = new Thread(new Runnable() {
		public void run() {
		    String onText = s1;
		    String doneText = s2;
		    String offText = "";
		    int count = 0;
		    try {
			while (blinking) {
			    Thread.sleep(500);
			    if (count%2 == 0) {
				status.setText(offText);
			    } else {
				status.setText(onText);
			    }
			    count++;
			}
		    } catch (InterruptedException ex) {
			status.setText(doneText);
		    }
		}
	    });
	blinking = true;
	blink.start();
	return blink;
    }
	
    /**
     * Display an error message in the progress label.
     * @param s The error message to display.
     */
    public void error(String s) {
	progress.setText(s);
    }	
	
    /**
     * Make a JTable and associated DocsTableModel for containing
     * query result sets.
     * @return The JTable with its model and listener initialized.
     */
    private JTable makeDocsTable() {
	DocsTableModel model = new DocsTableModel();
	JTable table = new JTable(model);
	table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	table.getSelectionModel().addListSelectionListener(new DocTableListener());
	TableColumn column;
	column = table.getColumnModel().getColumn(0);
	column.setPreferredWidth(50);
	column = table.getColumnModel().getColumn(1);
	column.setPreferredWidth(250);
	column = table.getColumnModel().getColumn(2);
	column.setPreferredWidth(30);
	column = table.getColumnModel().getColumn(3);
	column.setPreferredWidth(30);
	return table;
    }
	
    /**
     * Make a menu item with appropriate colors and <code>this</end> as its listener. 
     * @param label The label for the menu item
     * @return The menu item
     */
    private JMenuItem makeMenuItem(String label) {
	JMenuItem item 	= new JMenuItem(label);
	item.setForeground(navyBlue);
	item.setBackground(lightYellow);
	item.addActionListener(this);
	return item;
    }
    /**
     * Make the containing frame, menubar, and an instance of RetUI. Initialize.
     */
    private static void createAndShowGUI() {
	RetUI sim = new RetUI();      
	JFrame myFrame = new JFrame("Indri -- Retrieval");
	myFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	sim.setOpaque(true); //content panes must be opaque
	myFrame.getContentPane().add(sim, BorderLayout.CENTER);
	myFrame.setForeground(navyBlue);
	myFrame.setBackground(lightYellow);
	myFrame.getContentPane().setForeground(navyBlue);
	myFrame.getContentPane().setBackground(lightYellow);
	sim.init();
	myFrame.setSize(800, 500);
		
	JMenuBar mb = new JMenuBar();
	mb.setForeground(navyBlue);
	mb.setBackground(lightYellow);	
	JMenu m = new JMenu("File");
	m.setForeground(navyBlue);
	m.setBackground(lightYellow);
	m.add(sim.makeMenuItem("Add Index"));
	m.add(sim.makeMenuItem("Add Server"));
	// done as a menu item is not as nice as a right click
	// separate Remove button...
	m.add(sim.makeMenuItem("Remove Selected Index"));
	m.add(sim.makeMenuItem("Exit"));      
	mb.add(m);
	JMenu helpMenu = new JMenu("Help");
	helpMenu.setForeground(navyBlue);
	helpMenu.setBackground(lightYellow);
	helpMenu.add(sim.makeMenuItem("Help"));
	helpMenu.add(sim.makeMenuItem("About"));
	mb.add(Box.createHorizontalGlue());
	mb.add(helpMenu);
	myFrame.setJMenuBar(mb);
	myFrame.pack();
	myFrame.setVisible(true);
    }
	
    /**
     * Fire it up.
     * @param args ignored
     */
    public static void main(String [] args) {
	//Schedule a job for the event-dispatching thread:
	//creating and showing this application's GUI.
	javax.swing.SwingUtilities.invokeLater(new Runnable() {
		public void run() {
		    createAndShowGUI();
		}
	    });
    }
}

/**
 * Table Model for the scored document results from running a query.
 * @author David Fisher
 *
 */
class DocsTableModel extends AbstractTableModel {
    /**
     * Labels for the columns
     */
    // start and end are uninteresting. internal id/title could be more useful
    private String[] columnNames = {"Score", "Document", "Start", "End" };
    /**
     * Container for the data.
     */
    private Object[][] data = new Object[0][0];
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getColumnCount()
     */
    public int getColumnCount() {
	return columnNames.length;
    }
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getRowCount()
     */
    public int getRowCount() {
	return data.length;
    }
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getColumnName(int)
     */
    public String getColumnName(int col) {
	return columnNames[col];
    }
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getValueAt(int, int)
     */
    public Object getValueAt(int row, int col) {
	return data[row][col];
    }
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#getColumnClass(int)
     */
    public Class getColumnClass(int c) {
	return getValueAt(0, c).getClass();
    }
	
    /**
     * Resize the data array to contain <code>row</code> number of rows.
     * @param row Number of rows for the new array.
     */
    public void resize(int row) {
	data = new Object[row][columnNames.length];
    }
	
    /**
     * Clear the data array.
     */
    public void clear() {
	int last = data.length - 1;
	if (last >= 0) {
	    data = new Object[0][columnNames.length];
	    fireTableRowsDeleted(0, last);
	}
    }
	
    /* (non-Javadoc)
     * @see javax.swing.table.TableModel#setValueAt(java.lang.Object, int, int)
     */
    public void setValueAt(Object obj, int row, int col) {
	data[row][col] = obj;
	fireTableCellUpdated(row, col);
    }
    /**
     * 	Update an entry for a row
     * @param row The row index
     * @param score The document score
     * @param doc The document name
     * @param start The start offset of the extent
     * @param end The end offset of the extent
     */
    public void setValueAt(int row, double score, String doc, int start, 
			   int end) {
	data[row][0] = new Double(score);
	data[row][1] = doc;
	data[row][2] = new Integer(start);
	data[row][3] = new Integer(end);
	fireTableRowsUpdated(row, row);
    }
}
