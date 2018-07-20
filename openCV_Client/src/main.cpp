/*
 *    _____ _      _____ ______ _   _ _______
 *   / ____| |    |_   _|  ____| \ | |__   __|
 *  | |    | |      | | | |__  |  \| |  | |
 *  | |    | |      | | |  __| | . ` |  | |
 *  | |____| |____ _| |_| |____| |\  |  | |
 *   \_____|______|_____|______|_| \_|  |_|
 *
 */


/*
 * Optionen ueber argc argv einfuegen
 * 	- Bot: Client zeigt es einfach an
 *  - Follow the Leader: Client erkennt IR-LED und Richtet die Fahrrichtung
 *  	danach aus (Servos muessen starr sein)
 *  - Sentry: Camera ist starr, soll bewegte Gegenstaende erkennen und diese
 *  	immer im Mittelpunkt behalten. Bot Bewegung wird anhand der Camera
 *  	Commands angepasst.
 *
 *
 *
 */

#include "../includes/clientClass.hpp"
#include "../includes/cameraClass.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;






int main(int argc, char** argv)
{
	Client cClient;
    //--------------------------------------------------------
    //networking stuff: socket , connect
    //--------------------------------------------------------
    int sokt, option;
    char* serverIP;
    int serverPort;

    while ((option = getopt(argc, argv, "t:h"))!=-1) {
    	switch(option) {
    	case 't':
    		serverIP   = argv[2];
    		serverPort = atoi(argv[3]);

    		//cout << "Connect to... " << serverIP << ":" << serverPort << endl;
			cClient.writeLogFile("Connect to", serverIP, INFO);
    		break;
    	case 'h':
    		cout << "Help Text" << endl;
    		break;
    	case '?':
    		if (optopt == 't')
    			cClient.writeLogFile("Option -t requires an argument -> IPv4 Port.",
    					NULL, ERROR);
    		else if (isprint (optopt))
    			cClient.writeLogFile("Unknown Option", NULL, ERROR);
    		else
    			cClient.writeLogFile("Unknown character", std::to_string(optopt),
    					ERROR);
    		return 1;
    	default:
    		cClient.writeLogFile("Wrong Arguments entered", NULL, ERROR);
    		return 2;
    	}
    }

//    if (argc < 3) {
//           cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << endl;
//           return 3;
//    }



    struct  sockaddr_in serverAddr;
    socklen_t           addrLen = sizeof(struct sockaddr_in);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    	cClient.writeLogFile("socket() failed!", NULL, ERROR);
        return 3;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
    	cClient.writeLogFile("connect() failed!", NULL, ERROR);
        return 4;
    }



    //----------------------------------------------------------
    //OpenCV Code
    //----------------------------------------------------------

    Mat img;
    int height = 480;
    int width = 640;
    img = Mat::zeros(height , width, CV_8UC1);
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes = 0;
    int key;
    string displayedText;

    //make img continuos
    if ( ! img.isContinuous() ) {
          img = img.clone();
    }

    //cout << "Image Size:" << imgSize << endl;
	cClient.writeLogFile("Image Size", std::to_string(imgSize), INFO);
    displayedText = "Resolution: " +  to_string(height) + ", " + to_string(width);

    namedWindow("CV Video Client",1);

    /*
     * Search Algorithm
     */
    // create Selective Search Segmentation Object using default parameters
    //Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
    // set input image on which we will run segmentation
    //ss->setBaseImage(img);
    //ss->switchToSelectiveSearchFast();

    // run selective search segmentation on input image
	vector<Rect> rects;
	//ss->process(rects);
	cout << "Total Number of Region Proposals: " << rects.size() << endl;

	int numShowRects = 100;

    while (key != 'q') {

        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
        	cClient.writeLogFile("recv failed, received bytes",
        			std::to_string(bytes), ERROR);
        }

        /*
         * Status-Bar
         */
        cv::rectangle(img, Point(0,0), Point(width,10), cv::Scalar(0,0,0),
        		CV_FILLED,1,0);
        cv::putText(img, displayedText, Point(2,10), FONT_HERSHEY_PLAIN,
				1, (0, 255, 255));

        /*
         * Cross
         */
        cv::line(img, Point(0,(height/2)), Point(width,(height/2)),
        		cv::Scalar(255,255,255),1,1,0);
        cv::line(img, Point((width/2),0), Point((width/2),height),
                		cv::Scalar(255,255,255),1,1,0);

        /*
         * Draw search-rectangles
         */
        for(unsigned int i = 0; i < rects.size(); i++) {
            if (i < numShowRects) {
                cv::rectangle(img, rects[i], Scalar(0, 255, 0));
            }
            else {
                break;
            }
        }




        cv::imshow("CV Video Client", img);

        if (key = cv::waitKey(10) >= 0) break;
    }

    close(sokt);

    return 0;
}
