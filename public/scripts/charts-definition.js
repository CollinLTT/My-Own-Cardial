// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartT = createECGChart();
}

// Create Temperature Chart
function createECGChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-ecg',
      type: 'line' 
    },
    series: [
      {
        name: 'Pulse'
      }
    ],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: false,
        dataLabels: { 
          enabled: false 
        }
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Pulse Strength' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}