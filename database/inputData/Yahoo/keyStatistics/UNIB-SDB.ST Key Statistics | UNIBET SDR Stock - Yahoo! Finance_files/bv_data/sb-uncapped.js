var sb = {"pixels":[70],"BR_APS":"3VDl9az52vCEB2yDZVg"};
var delay = 0;
for(var i=0;i<sb.pixels.length;i++) {
if (i % 3 == 0 && i != 0)
delay += 200;
(function (i) {
setTimeout(function () {
(new Image()).src='https'+'://'+'geo-um.btrll.com/v1/map_pixel/sb/'+sb.pixels[i]+'.png';
}, delay);
})(i);
}
