    # importing required packages
    import cv2
    import numpy as np
     
     
     
    X_WINDOW_HEIGHT = 1024
    Y_WINDOW_HEIGHT = 512
    # mouse call back function
    # def click_event(event, x, y,
    #                 flags, params):
        
    #     # if the left button of mouse 
    #     # is clicked
    #     if event == cv2.EVENT_LBUTTONDOWN:
          
           
          
        #     points.append((x,y))
        #     f.write(f"{x} {y}\n")
          
        #     # marking the point with a circle
        #     # of center at that point and 
        #     # small radius
        #     cv2.circle(img,(x,y), 5, 
        #                (0, 255, 0), -1)
              
        #     # if length of points list 
        #     # greater than2 then this
        #     # condition executes
        #     if len(points) >= 2:
                
        #         # joins the current point and
        #         # the previous point in the
        #         # list with a line
        #         cv2.line(img, points[-1], points[-2],
        #                  (0, 255, 255), 2)
                  
        #     # displays the image
        #     cv2.imshow('image', img)
     
        
        # if event == cv2.EVENT_RBUTTONDOWN:
        #     cv2.line(img, points[-1], points[0],(0, 255, 255), 2)
        #     cv2.imshow('image', img)
            
    # making an black image
    # of size (512, 1024, 3)
    # create 3-d numpy 
    # zeros array
     
    f = open("inputCoordinates.txt", "r")
    # print(f.readlines())
     
    noOfVertices = int(f.readline().replace('\n','')) # To get How many vertices the polygon has
     
    inputCoordinates = []
     
    for x in range(noOfVertices): # loop runs from 0 to noOfVertices-1 ( so noOfVertices times.)
       
        line = f.readline()
        x = (line.replace('\n','')).split()
        temp = []
        for xx in x:
            temp.append(int(xx))
        inputCoordinates.append(temp)
     
     
     
     
    img = np.zeros((512, 1024, 3),
                 np.uint8)
     
    font = cv2.FONT_HERSHEY_SIMPLEX
     
    # declare a list to append all the 
    # points on the image we clicked 
    points = []
    prevX = inputCoordinates[-1][0]
    prevY = inputCoordinates[-1][1]
     
    for x,y in inputCoordinates:
        cv2.circle(img,(x,Y_WINDOW_HEIGHT - y), 5,(0, 255, 0), -1)
     
       
        cv2.line(img, [x,Y_WINDOW_HEIGHT - y], [prevX,Y_WINDOW_HEIGHT - prevY],(0, 255, 255), 2)  
        prevX = x
        prevY = y
     
    # show the image
    cv2.imshow('image',img)
    # time.sleep(2)
    f.close()
    f = open("outputCoordinates.txt", "r")
     
     
    noOfPolygons = int(f.readline().replace('\n',''))
    counter = 0
     
     
    def click_event(event, x, y,
                    flags, params):
        
        if event != cv2.EVENT_LBUTTONDOWN:
            return
        global counter
        counter = counter + 1
        
        if counter < noOfPolygons:
            noOfVertices = int(f.readline().replace('\n',''))
     
            vertices = []
     
            for x in range(noOfVertices):
                line = f.readline()
                x = (line.replace('\n','')).split()
                temp = [int(x[0]),int(x[1])]
                vertices.append(temp)
            prevX = vertices[-1][0]
            prevY = vertices[-1][1]
     
            for vertex in vertices:
     
                x,y = vertex
                # print(x,y,prevX,prevY)
                cv2.line(img, [x,Y_WINDOW_HEIGHT - y], [prevX,Y_WINDOW_HEIGHT -prevY],(0, 255, 255), 2)
                cv2.imshow('image',img)
                prevY = y
                prevX = x
     
            
        else:
            cv2.putText(img, "Done", (0,500), font,1, (0, 0, 255), 2)
            cv2.imshow('image',img)
       
            
     
    cv2.setMouseCallback('image',
                         click_event, 1)
     
    cv2.imshow('image',img)
    # setting mouse call back
    # cv2.setMouseCallback('image',
    #                      click_event)
     
     
    # no waiting
    cv2.waitKey(0)
     
     
    # To close the image
    # window and file that we opened
    f.close()
    cv2.destroyAllWindows()      