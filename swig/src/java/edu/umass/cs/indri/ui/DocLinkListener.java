package edu.umass.cs.indri.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.io.IOException;
import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

/**
 * Simple pop an internalframe for hyperlinks.
 * Spawns a new JFrame to hold the individuals.
 * @author David Fisher
 */
class DocLinkListener implements HyperlinkListener {
	/**
	 * Main container frame for hyperlinks.
	 */
	JFrame f;
	/**
	 * Desktop pane for internal frames.
	 */
	JDesktopPane deskTop;
	/**
	 * Number of windows displayed in the frame.
	 */
	int winCount;
	/**
	 * Make a link listener.
	 */
	public DocLinkListener() {
		winCount = 0;
		f = new JFrame("Web Links");
		deskTop = new JDesktopPane();
		deskTop.setPreferredSize(new Dimension(700, 500));
		f.setContentPane(deskTop);
		f.pack();
		// fix the icon
		//	    f.setIconImage(createImageIcon(iconFile).getImage());
		f.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
	}
	
	
	
	/* (non-Javadoc)
	 * @see javax.swing.event.HyperlinkListener#hyperlinkUpdate(javax.swing.event.HyperlinkEvent)
	 */
	public void hyperlinkUpdate(HyperlinkEvent e) {
		if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
			java.net.URL link = e.getURL();
			JInternalFrame iframe = new JInternalFrame(e.getDescription(),
					true,
					true,
					true,
					true);
			JTextPane doc = new JTextPane();
			doc.setPreferredSize(new Dimension(500, 350));
			doc.setEditable(false);
			doc.addHyperlinkListener(this);
			JScrollPane scroller = 
				new JScrollPane(doc, 
						JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
						JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
			scroller.setPreferredSize(new Dimension(600, 400));
			JPanel p = new JPanel();
			p.setOpaque(true);
			p.add(scroller);
			
			try {doc.setPage(link);
			} catch (IOException ex) {
				return;
			}
			iframe.getContentPane().add(p, BorderLayout.CENTER);
			iframe.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			iframe.pack();
			//Set the window's location.
			iframe.setLocation(30*winCount, 30*winCount);
			winCount++;
			iframe.setVisible(true);
			deskTop.add(iframe);
			try {
				iframe.setSelected(true);
			} catch (java.beans.PropertyVetoException v) {
				// don't care.
			}
			f.setVisible(true);
		}
	}
}